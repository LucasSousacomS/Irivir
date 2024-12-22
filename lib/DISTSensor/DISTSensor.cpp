#include <DISTSensor.h>


bool DISTSensor::measureClosing(){
    uint8_t minDiff = 10;
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    uint16_t lastMeasure = measure.RangeMilliMeter;
    bool closing = 0;
    for(int i = 0; i<=11; i++){
        lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
        if(measure.RangeStatus == 4){
            continue;
        }
        if(measure.RangeMilliMeter <= lastMeasure-minDiff){
            if(closing == 0){
                closing = 1;
                lastMeasure = measure.RangeMilliMeter; 
            }
            else{
                return 1;
            }        
        }
        delay(1);
    }
    return 0;
}

uint16_t DISTSensor::getDistance(){
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        return measure.RangeMilliMeter;
    } else {
        return 6000;
    }
}

void DISTSensor::begin(){
    if(!lox.begin()){
        Serial.begin(9600);
        Serial.print("Conexão com o sensor de distância falhou");
        Serial.end();
        while (true)
        {
            /* code */
        }
        
    }else{
        Serial.println("Sensor de distãncia inicializado corretamente");
    }
}
