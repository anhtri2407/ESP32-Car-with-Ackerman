#include <Servo.h>
#include "BluetoothSerial.h"
/*
        Xe Siêu Cấp Vũ Trụ
        Created: 10 Feb 2023
        Author: STEM CLUB
*/

//Bluetooth setup check if enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Servo setup
const int servo_pin = 15;

//Motor setup
// const int PWM, AIN1, AIN2;


Servo Servo;
BluetoothSerial BTSerial;

void moveServo(void * parameter) {
  while (true) {
    for (int angle = 0; angle < 180; angle++) {
        Servo.write(angle);
        Serial.println("Current Servo's angle written: " + String(angle));
      }
      vTaskDelay(100 / portTICK_PERIOD_MS);
      for (int angle = 180; angle > 0; angle--) {
        Servo.write(angle);
        Serial.println("Current Servo's angle written: " + String(angle));
      }
      vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// void counting(void * parameter) {
//   while (true) {
//       for (int i = 0; i <= 5; i++) {
//       Serial.println(i);
//       vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
//   }
// }

void setup() {
  Serial.begin(115200);
  BTSerial.begin("ESP32 Bluetooth Test");
  Servo.attach(servo_pin);
  Servo.write(90);
  xTaskCreatePinnedToCore(moveServo, 
                          "Move Servo", 
                          2048, 
                          NULL, 
                          10, 
                          NULL, 
                          0);
  // xTaskCreate(counting,
  //             "Couting Function",
  //             1000,
  //             NULL,
  //             10,
  //             NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println(xPortGetCoreID());
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
  if (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
  delay(20);
}
  Servo.attach(servo_pin);
  Servo.write(90);
  xTaskCreatePinnedToCore(moveServo, 
                          "Move Servo", 
                          1000, 
                          NULL, 
                          10, 
                          NULL, 
                          app_cpu);
}

// void setup() {}

// void loop() {}