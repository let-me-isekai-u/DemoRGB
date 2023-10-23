from flask import Flask, render_template, request
import Adafruit_DHT
import Adafruit_CharLCD as LCD
import RPi.GPIO as GPIO
import time

app = Flask(__name__)
GPIO.setmode(GPIO.BCM)

# Cấu hình GPIO cho đèn RGB và LCD
RED_PIN = 17
GREEN_PIN = 18
BLUE_PIN = 22

GPIO.setup(RED_PIN, GPIO.OUT)
GPIO.setup(GREEN_PIN, GPIO.OUT)
GPIO.setup(BLUE_PIN, GPIO.OUT)

# Cấu hình LCD
lcd_rs = 25
lcd_en = 24
lcd_d4 = 23
lcd_d5 = 17
lcd_d6 = 18
lcd_d7 = 22
lcd_columns = 16
lcd_rows = 2

lcd = LCD.Adafruit_CharLCD(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7, lcd_columns, lcd_rows)

# Trang chủ
@app.route('/')
def index():
    return render_template('RGBcontrol.html')

# API để nhận dữ liệu từ cảm biến DHT22 và điều khiển đèn RGB và LCD
@app.route('/update', methods=['POST'])
def update():
    humidity, temperature = Adafruit_DHT.read_retry(Adafruit_DHT.DHT22, 4)  # GPIO 4

    # Hiển thị nhiệt độ và độ ẩm trên LCD
    lcd.clear()
    lcd.message(f'Temp: {temperature:.2f}C\nHumidity: {humidity:.2f}%')

    # Điều khiển đèn RGB dựa trên nhiệt độ
    if temperature is not None:
        if temperature < 25:
            GPIO.output(RED_PIN, GPIO.LOW)
            GPIO.output(GREEN_PIN, GPIO.LOW)
            GPIO.output(BLUE_PIN, GPIO.HIGH)  # Blue
        else:
            GPIO.output(RED_PIN, GPIO.HIGH)  # Red
            GPIO.output(GREEN_PIN, GPIO.LOW)
            GPIO.output(BLUE_PIN, GPIO.LOW)
    else:
        # Nếu không đọc được nhiệt độ, tắt tất cả các đèn
        GPIO.output(RED_PIN, GPIO.LOW)
        GPIO.output(GREEN_PIN, GPIO.LOW)
        GPIO.output(BLUE_PIN, GPIO.LOW)
    
    return f'Temperature: {temperature:.2f}°C, Humidity: {humidity:.2f}%'

if __name__ == '__main__':
    try:
        app.run(debug=True, host='0.0.0.0')
    except KeyboardInterrupt:
        pass
    finally:
        lcd.clear()
        GPIO.cleanup()
