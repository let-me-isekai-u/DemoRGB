import Adafruit_DHT
import RPi.GPIO as GPIO
import time

# Thiết lập chân GPIO cho cảm biến DHT22 và LED RGB
DHT_PIN = 4  # Chân GPIO dành cho cảm biến DHT22
LED_PIN_R = 17  # Chân GPIO dành cho đèn LED RGB màu đỏ
LED_PIN_G = 27  # Chân GPIO dành cho đèn LED RGB màu xanh
LED_PIN_B = 22  # Chân GPIO dành cho đèn LED RGB màu xanh dương

# Thiết lập chế độ chân GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN_R, GPIO.OUT)
GPIO.setup(LED_PIN_G, GPIO.OUT)
GPIO.setup(LED_PIN_B, GPIO.OUT)

# Hàm đọc giá trị từ cảm biến DHT22
def read_dht22():
    humidity, temperature = Adafruit_DHT.read_retry(Adafruit_DHT.DHT22, DHT_PIN)
    return humidity, temperature

# Hàm điều khiển đèn LED theo giá trị nhiệt độ
def control_led(temperature):
    if temperature < 20:
        # Nhiệt độ dưới 20 độ Celsius: Đèn LED màu xanh
        GPIO.output(LED_PIN_R, GPIO.LOW)
        GPIO.output(LED_PIN_G, GPIO.HIGH)
        GPIO.output(LED_PIN_B, GPIO.LOW)
    elif 20 <= temperature < 25:
        # Nhiệt độ từ 20 đến 24 độ Celsius: Đèn LED màu vàng
        GPIO.output(LED_PIN_R, GPIO.HIGH)
        GPIO.output(LED_PIN_G, GPIO.HIGH)
        GPIO.output(LED_PIN_B, GPIO.LOW)
    else:
        # Nhiệt độ từ 25 độ Celsius trở lên: Đèn LED màu đỏ
        GPIO.output(LED_PIN_R, GPIO.HIGH)
        GPIO.output(LED_PIN_G, GPIO.LOW)
        GPIO.output(LED_PIN_B, GPIO.LOW)

try:
    while True:
        # Đọc giá trị từ cảm biến DHT22
        humidity, temperature = read_dht22()

        # Kiểm tra xem giá trị đọc được có hợp lý không (không bằng None)
        if humidity is not None and temperature is not None:
            print(f"Nhiệt độ: {temperature}°C, Độ ẩm: {humidity}%")
            
            # Điều khiển đèn LED dựa trên giá trị nhiệt độ
            control_led(temperature)
        else:
            print("Không thể đọc giá trị từ cảm biến.")

        # Chờ 2 giây trước khi đọc giá trị tiếp theo
        time.sleep(2)

except KeyboardInterrupt:
    # Khi nhấn Ctrl + C, thoát chương trình và dọn sạch GPIO
    GPIO.cleanup()
    print("Chương trình đã dừng.")