#include <DISTSensor.h>
#include <debug.h>

// Função para detectar algo se aproximando
bool DISTSensor::measureClosing(){
    uint8_t minDiff = 10; // Velocidade com que a coisa precisa estar se aproximando (diferença entre a posição atual do objeto e da leitura anterior)
    // VL53L0X_RangingMeasurementData_t measure; // Variável que irá guardar os valores de distância
    lox.rangingTest(&measure, false); // Obtendo leituras de distância (true salvaria também mensagens de debug)
    uint16_t lastMeasure = measure.RangeMilliMeter; // Variável usada para comparar valores de distância
    bool closing = 0; // Variável para identificar se há algo chegando
    for(int i = 0; i<=11; i++){ // Efetuar 11 leituras para identificar se há algo se aproximando
        lox.rangingTest(&measure, false); // Fazendo a leitura
        if(measure.RangeStatus == 4){ // Caso o rangeStatus seja 4, houve algum problema na leitura
            continue;
        }
        if(measure.RangeMilliMeter <= lastMeasure-minDiff){ // Caso a medida atual seja menor ou igual à anterior - 10
            if(closing == 0){ // Caso a variável closing seja 0, essa foi a primeira deteção
                closing = 1; // Faz closing virar 1, identificando que já houve uma deteção
                lastMeasure = measure.RangeMilliMeter; // Salva essa medida como a medida anterior para a próxima leitura
            }
            else{
                return 1;
            }        
        }
        lastMeasure = measure.RangeMilliMeter;
        delay(1);
        DEBUG_PRINTLN(measure.RangeMilliMeter);
    }
    return 0;
}

uint16_t DISTSensor::getDistance(){  
    // if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) { // Por algum motivo, quando a função "mind" precisa chamar essa função getDistance, é preciso reinicializar o objeto do sensor de dis
    delay(10);
    // lox.begin();  // Reiniciando o sensor por que POR ALGUM MOTIVO, na hora que a função "mind" chama a getDistance o sensor não está mais inicializado
    // }
    lox.rangingTest(&measure, false); // Leitura do sensor de distância
    if (measure.RangeStatus != 4) {  // Caso não haja falha na leitura
        DEBUG_PRINTLN(measure.RangeMilliMeter);
        return measure.RangeMilliMeter; // retorna a distância
    } else {
        return 6000; // Caso haja falha, retorna um valor qualquer
    }
    
}

u_int16_t* DISTSensor::getDistances(){
    DEBUG_PRINTLN("elas:");
    for(int i = 0; i<3; i++){
        DEBUG_PRINTLN(distances[i]);
    }
    return distances; // Retorna um array com 3 distâncias
}

void DISTSensor::setDistances(u_int8_t pos){ // Função para setar um valor no array distances[]
    // begin();
    if(pos < 3){ // Teste para ver se o índice selecionado está ok
        distances[pos] = getDistance(); // Leitura da distância pelo sensor de distância e preenchimento do array
    }
}

void DISTSensor::begin(){
    if(!lox.begin()){ // Inicialização do sensor de distância
        while (true)
        {
            
        }
    }else{
        DEBUG_PRINTLN("Sensor de distãncia inicializado corretamente");
    }
}
