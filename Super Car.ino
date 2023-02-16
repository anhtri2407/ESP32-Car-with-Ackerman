/*
        ESP32 Car with Ackerman Steering Mechanism
        Created: 10 Feb 2023
        Author: Anh Tri - STEM CLUB
        Device's Controller used: Bluetooth RC Joystick Controll - Andi.Co
        Status: Done
*/

#include <Servo.h>
#include <TB6612FNG.h>
#include "BluetoothSerial.h"

//enable classic bluetooth if not enabled & bluetooth setup
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial BTSerial;

//initialize variables
short temp, speed, angle; bool is_back_pressed;

//servo setup
const int SERVO_PIN = 15;
const int SERVO_CHANNEL = 10, SERVO_BASE_FREQUENCY = 500, SERVO_BITS = 10;
Servo Servo;

//motor setup
const int PWMA = 4, STANDBY = 5, AIN1 = 16, AIN2 = 17;
// const int MOTOR_CHANNEL = 1, MOTOR_BASE_FREQUENCY = 1000, MOTOR_BITS = 8;
Tb6612fng Motor(STANDBY, AIN2, AIN1, PWMA);

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
      vTaskDelay(3 / portTICK_PERIOD_MS);
  }
}

void driving(void * parameter) {
  while (true) {
    if (is_back_pressed) Motor.drive(-speed / 255.0); 
    else Motor.drive(speed / 255.0);
    vTaskDelay(3 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  BTSerial.begin("MRC's Car");
  Motor.begin();
  Servo.attach(SERVO_PIN, SERVO_CHANNEL, 0, 180);
  xTaskCreatePinnedToCore(read_variables,
                          "Read Variables",
                          2048,
                          NULL,
                          12,
                          NULL,
                          0); //using core 0 to reading variables from Bluetooth's signal
  xTaskCreatePinnedToCore(navigate_servo, 
                          "Navigate Servo", 
                          2048, 
                          NULL, 
                          10, 
                          NULL, 
                          0); //using core 0 to navigating servo's direction
  xTaskCreatePinnedToCore(driving, 
                          "Driving Car", 
                          2048, 
                          NULL, 
                          10, 
                          NULL, 
                          1); //using core 1 to driving car at the same time with servo
}

void loop() {} //Don't use
