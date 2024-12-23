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