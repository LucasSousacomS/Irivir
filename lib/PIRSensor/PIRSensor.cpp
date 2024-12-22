#include "PIRSensor.h"

PIRSensor::PIRSensor(uint8_t pin){
    this->pin = pin;
}

bool PIRSensor::detection(){
    if(digitalRead(pin) && (millis() - timeNow > 1000)){
        timeNow = millis();
        return 1;
    }
    else{
        return 0;
    }
}

void PIRSensor::begin(){
    pinMode(pin, INPUT_PULLDOWN);
}
