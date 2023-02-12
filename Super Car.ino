/*
        ESP32 Car with Ackerman Steering Mechanism
        Created: 10 Feb 2023
        Author: Anh Tri - STEM CLUB
        Device's Controller used: Bluetooth RC Joystick Controll - Andi.Co
*/

#include <Servo.h>
#include "BluetoothSerial.h"

//enable classic bluetooth if not enabled & bluetooth setup
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial BTSerial;

//initialize variables
short temp, speed, angle; bool is_back_pressed;

//servo setup
const int servo_pin = 15;
Servo Servo;

//motor setup
// const int PWM, AIN1, AIN2;

void read_variables(void * parameter) {
  while (true) {
    if (BTSerial.available()) {
      temp = BTSerial.read(); //useless variable
      speed = BTSerial.read();
      angle = BTSerial.read();
      is_back_pressed = BTSerial.read();
    // Serial.println("TEMP: " + String(temp) + ", SPEED: " + String(speed) + ", ANGLE: " + String(angle) + ", IS_BACKED_PRESSED: " + String(is_back_pressed)); //Chỉ uncomment khi muốn debug
    }
    vTaskDelay(3 / portTICK_PERIOD_MS);
  }
}

void navigate_servo(void * parameter) {
  while (true) {
      Servo.write(angle);
      // Serial.println("Current Servo's angle written: " + String(angle)); //Chỉ uncomment khi muốn debug
      // vTaskDelay(3 / portTICK_PERIOD_MS);
  }
}

void driving(void * parameter) {
  while (true) {
    if (is_back_pressed) {
      //do something here
    } else {
      //do something here
    }
  }
}

void setup() {
  Serial.begin(115200);
  BTSerial.begin("MRC's Car");
  // Servo.attach(servo_pin);
  // Servo.write(90);
  xTaskCreatePinnedToCore(read_variables,
                          "Read Variables",
                          2048,
                          NULL,
                          12,
                          NULL,
                          0); //using core 0 to reading variables from Bluetooth's signal
  xTaskCreatePinnedToCore(navigate_servo, 
                          "Navigate Servo", 
                          1024, 
                          NULL, 
                          10, 
                          NULL, 
                          1); //using core 1 to navigating servo's direction
}

void loop() {}
