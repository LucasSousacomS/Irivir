#include <DISTSensor.h>


bool DISTSensor::measureClosing(){
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    uint16_t lastMeasure = measure.RangeMilliMeter;
    bool closing = 0;
    for(int i = 0; i<=11; i++){
        lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
        if(measure.RangeStatus == 4){
            continue;
        }
        if(measure.RangeMilliMeter <= lastMeasure-10){
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
