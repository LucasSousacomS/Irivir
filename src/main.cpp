#include <Arduino.h>
#include <PIRSensor.h>
#include "driver/rtc_io.h"
#include <actuators.h>

PIRSensor pir(4);
DISTSensor dist;
Car car;
Vision vision;

void setup() {
  
  Serial.begin(9600);
  dist.begin();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);
  rtc_gpio_pullup_dis(GPIO_NUM_4);
  rtc_gpio_pulldown_en(GPIO_NUM_4);
  Serial.println("Causa: " + String(esp_sleep_get_wakeup_cause()));
  // Reinitialize sensor if waking from deep sleep
  
  if((dist.measureClosing() && dist.getDistance() <= 300) || dist.getDistance() <= 100){
    car.mind();
  }
  delay(100);
  esp_deep_sleep_start();
}

void loop() {
}
