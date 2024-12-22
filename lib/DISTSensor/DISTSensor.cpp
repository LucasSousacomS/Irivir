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
    Serial.println("Dentro do getDistance");
    VL53L0X_RangingMeasurementData_t measure;
    Serial.println("Criou a variavel");
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) { // PPor algum motivo, quando a função "mind" precisa chamar essa função getDistance, é preciso reinicializar o objeto do sensor de distância
        Serial.println("Nada");
        lox.begin();  // Reinitialize the sensor after wake up
    }
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    Serial.println("Prestes a entrar no if"); 
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.println("Dentro do if do getDistance");
        return measure.RangeMilliMeter;
    } else {
        return 6000;
    }
}

u_int16_t* DISTSensor::getDistances(){
    return distances;
}

void DISTSensor::setDistances(u_int8_t pos){
    if(pos < 3){
        Serial.print("Dentro do if do setDistances. Posição: ");
        Serial.println(pos);
        distances[pos] = getDistance();
        Serial.print("Distância salva: ");
        Serial.println(distances[pos]);
    }else{
        Serial.println("bug");
    }
}

void DISTSensor::begin(){
    if(!lox.begin()){
        while (true)
        {
            /* code */
        }
    }else{
        Serial.println("Sensor de distãncia inicializado corretamente");
    }
}
