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
  // Reinitialize sensor if waking from deep sleep
  
  if(dist.measureClosing()){
    if(dist.getDistance() <= 100){
      car.mind();
    };
  }
  esp_deep_sleep_start();
}

void loop() {
  Serial.println(dist.measureClosing());
}
