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
  vision.begin();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);
  rtc_gpio_pullup_dis(GPIO_NUM_4);
  rtc_gpio_pulldown_en(GPIO_NUM_4);
  Serial.println("Causa: " + String(esp_sleep_get_wakeup_cause()));
  // Reinitialize sensor if waking from deep sleep
  
  if((dist.measureClosing() && dist.getDistance() <= 300) || dist.getDistance() <= 100){
    car.mind(dist); // Chamada da função responsável por fazer o controle do carrinho (e passando o objeto dist, para permitir que o mesmo objeto seja manipulado por todo o programa)
  }
  delay(100);
  esp_deep_sleep_start();
}

void loop() {
}
