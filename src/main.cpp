#include <Arduino.h>
#include <PIRSensor.h>
#include "driver/rtc_io.h"
#include <actuators.h>
#include "esp_wifi.h"
#include <debug.h>

PIRSensor pir(4);
DISTSensor dist;
Car car;
Vision vision;

void setup() {  
  esp_wifi_stop();
  esp_wifi_deinit();
  Serial.begin(9600);
  
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);
  rtc_gpio_pullup_dis(GPIO_NUM_4);
  rtc_gpio_pulldown_en(GPIO_NUM_4);
  DEBUG_PRINTLN("Causa: " + String(esp_sleep_get_wakeup_cause()));
  // Reinitialize sensor if waking from deep sleep
  dist.begin();
  car.begin();
  
  if((dist.measureClosing() && dist.getDistance() <= 600) || dist.getDistance() <= 100){
    car.mind(dist); // Chamada da função responsável por fazer o controle do carrinho (e passando o objeto dist, para permitir que o mesmo objeto seja manipulado por todo o programa)
  }
  delay(100);
  esp_deep_sleep_start();
}

void loop() {
  
}
