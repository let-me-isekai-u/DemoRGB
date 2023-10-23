from flask import Flask, render_template, request
import Adafruit_DHT
import RPi.GPIO as GPIO

app = Flask(__name__)
GPIO.setmode(GPIO.BCM)

# Cấu hình GPIO cho đèn RGB
RED_PIN = 17
GREEN_PIN = 18
BLUE_PIN = 22

GPIO.setup(RED_PIN, GPIO.OUT)
GPIO.setup(GREEN_PIN, GPIO.OUT)
GPIO.setup(BLUE_PIN, GPIO.OUT)

# Trang chủ
@app.route('/')
def index():
    return render_template('index.html')

# API để nhận dữ liệu từ cảm biến DHT22 và điều khiển đèn RGB
@app.route('/update', methods=['POST'])
def update():
    humidity, temperature = Adafruit_DHT.read_retry(Adafruit_DHT.DHT22, 4)  # GPIO 4
    color = request.form['color']
    
    # Điều khiển đèn RGB
    if color == 'red':
        GPIO.output(RED_PIN, GPIO.HIGH)
        GPIO.output(GREEN_PIN, GPIO.LOW)
        GPIO.output(BLUE_PIN, GPIO.LOW)
    elif color == 'green':
        GPIO.output(RED_PIN, GPIO.LOW)
        GPIO.output(GREEN_PIN, GPIO.HIGH)
        GPIO.output(BLUE_PIN, GPIO.LOW)
    elif color == 'blue':
        GPIO.output(RED_PIN, GPIO.LOW)
        GPIO.output(GREEN_PIN, GPIO.LOW)
        GPIO.output(BLUE_PIN, GPIO.HIGH)
    else:
        GPIO.output(RED_PIN, GPIO.LOW)
        GPIO.output(GREEN_PIN, GPIO.LOW)
        GPIO.output(BLUE_PIN, GPIO.LOW)
    
    return f'Temperature: {temperature:.2f}°C, Humidity: {humidity:.2f}%'

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
