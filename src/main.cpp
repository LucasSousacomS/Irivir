#include <Arduino.h>
#include <PIRSensor.h>
#include "driver/rtc_io.h"
#include <DISTSensor.h>

PIRSensor pir(4);
DISTSensor dist;

void setup() {
  Serial.begin(9600);
  dist.begin();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);
  rtc_gpio_pullup_dis(GPIO_NUM_4);
  rtc_gpio_pulldown_en(GPIO_NUM_4);
  delay(1);
}

void loop() {
  Serial.println(dist.measureClosing());
}
