#include <actuators.h>
#define servoPin 13
#define servoDelay 7

Servo servo1;

void Vision::begin(){
    servo1.attach(servoPin);
    servo1.write(90);
}

void Vision::eyeLeft(DISTSensor& dist){
    for(int posDegrees = servo1.read(); posDegrees <= 180; posDegrees++) {
        servo1.write(posDegrees);
        delay(servoDelay);
    }
    dist.setDistances(0); // Setar o valor do índice 0 da variável distâncias
}

void Vision::eyeRight(DISTSensor& dist){
    for(int posDegrees = servo1.read(); posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
        delay(servoDelay);
    }
    dist.setDistances(2);
}

void Vision::eyeCenter(DISTSensor& dist){
    if(servo1.read() > 90){
        for(int posDegrees = servo1.read(); posDegrees >= 90; posDegrees--) {
        servo1.write(posDegrees);
        delay(servoDelay);
    }
    }else if(servo1.read()<90){
        for(int posDegrees = servo1.read(); posDegrees <= 90; posDegrees++) {
            servo1.write(posDegrees);
            delay(servoDelay);
        }
    }
    dist.setDistances(1);
}

void Vision::reading(DISTSensor& dist){ // Leitura de valores de distância à esqueda, centro e direita, preenchendo os índices do array
    // dist.begin(); // Precia chamar a função begin de novo pois estou usando um outro objeto para controlar o sensor agora (este dist não é o mesmo dist do main)
    eyeLeft(dist);
    eyeCenter(dist);
    eyeRight(dist);
    eyeCenter(dist);       
}

u_int16_t* Vision::getDistances(DISTSensor& dist){ // Obtenção do array distance[]
    return dist.getDistances();
}

Car::Direction Car::decideDirection(u_int16_t* distances){
    int smallestIndex = 0; // Variável para comparação de valores do array

    // Encontrar o menor valor no array de distâncias
    for (int i = 1; i < 3; i++) {
        Serial.println("Distancias");
        Serial.println(distances[i]);
        if (distances[i] < distances[smallestIndex]) {
            Serial.println("Distância lida:" + String(distances[i] + "na posição" + String(i)));
            smallestIndex = i;
        }
    }

    // Decidir para onde virar
    if (distances[smallestIndex] <= 500) {
        Serial.println("A menor Distância lida:" + String(distances[smallestIndex]));
        if (smallestIndex == 0) return right; // O valor lido quando o sensor está na esquerda (eyeLeft) é guardado no índice 0
        if (smallestIndex == 2) return left; // O valor lido quando o sensor está na direita (eyeRight) é guardado no índice 2
    }
    return straight; // Padrão se não estiver perto de nada
}

void Car::turn(Direction dir){
    if(dir == left){ 
        Serial.print("Virar à esquerda");
    }else{
        Serial.print("Virar à direita");
    }
}

void Car::forward(){
    Serial.println("Frente");
}

void Car::backward(){ // Função para fazer o carrinho andar pra trás
    Serial.println("Trás");
}

void Car::mind(DISTSensor& dist){
    u_int16_t now = millis();
    while (millis() - now < 10000){
        vis.reading(dist);
        u_int16_t* distances =  vis.getDistances(dist);
        Direction dir = decideDirection(distances);
        if(dir != straight) turn(dir);
        else forward();
    }
}