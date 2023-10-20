#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <DHT.h>

// Định nghĩa chân GPIO
#define DHTPIN 4      // Chân GPIO dành cho cảm biến DHT22
#define LED_PIN_R 0   // Chân GPIO dành cho đèn LED RGB màu đỏ
#define LED_PIN_G 2   // Chân GPIO dành cho đèn LED RGB màu xanh
#define LED_PIN_B 3   // Chân GPIO dành cho đèn LED RGB màu xanh dương

// Khởi tạo cảm biến DHT22
#define DHTTYPE DHT22

// Khởi tạo đèn LED RGB
void setupLED() {
    pinMode(LED_PIN_R, OUTPUT);
    pinMode(LED_PIN_G, OUTPUT);
    pinMode(LED_PIN_B, OUTPUT);
}

// Hàm điều khiển đèn LED theo giá trị nhiệt độ
void controlLED(float temperature) {
    if (temperature < 20) {
        digitalWrite(LED_PIN_R, LOW);
        digitalWrite(LED_PIN_G, HIGH);
        digitalWrite(LED_PIN_B, LOW);
    } else if (temperature >= 20 && temperature < 25) {
        digitalWrite(LED_PIN_R, HIGH);
        digitalWrite(LED_PIN_G, HIGH);
        digitalWrite(LED_PIN_B, LOW);
    } else {
        digitalWrite(LED_PIN_R, HIGH);
        digitalWrite(LED_PIN_G, LOW);
        digitalWrite(LED_PIN_B, LOW);
    }
}

int main(void) {
    if (wiringPiSetup() == -1) {
        printf("Không thể thiết lập WiringPi. Hãy chạy với quyền sudo.\n");
        return 1;
    }

    DHT dht(DHTPIN, DHTTYPE);
    setupLED();

    while (true) {
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        if (!isnan(humidity) && !isnan(temperature)) {
            printf("Nhiệt độ: %.2f°C, Độ ẩm: %.2f%%\n", temperature, humidity);
            controlLED(temperature);
        } else {
            printf("Không thể đọc giá trị từ cảm biến.\n");
        }

        delay(2000);  // Chờ 2 giây trước khi đọc giá trị tiếp theo
    }

    return 0;
}
