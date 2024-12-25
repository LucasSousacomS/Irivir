#include <actuators.h>
#define servoPin 13
#define servoDelay 7
#define pwmChannel1 2
#define pwmChannel2 3

#define motor1Pin1  27
#define motor1Pin2  26
#define motor2Pin1  25
#define motor2Pin2  33
#define enable1Pin  14
#define enable2Pin  12


uint8_t resolution = 8;
uint32_t freq = 30000;
int dutyCycle = 200;
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
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH);
        for(int i = 100; i<255; i++){
            ledcWrite(pwmChannel2, i);
            delay(1);
        }
    }else{
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW);
        for(int i = 100; i<255; i++){
            ledcWrite(pwmChannel1, i);
            delay(1);
        }
    }
}

void Car::forward(){
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    for(int i = 100; i<255; i++){
        ledcWrite(pwmChannel1, i);
        ledcWrite(pwmChannel2, i);
        delay(1);
    }
}

void Car::backward(){ // Função para fazer o carrinho andar pra trás
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    for(int i = 100; i<255; i++){
        ledcWrite(enable1Pin, i);
        ledcWrite(enable2Pin, i);
        delay(1);
    }
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

void Car::begin(){
    Serial.println("Car initialized");
    vis.begin();
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(enable1Pin, OUTPUT);
    pinMode(enable2Pin, OUTPUT);

    ledcSetup(pwmChannel1, freq, resolution);
    ledcSetup(pwmChannel2, freq, resolution);
    
    // configure LEDC PWM
    ledcAttachPin(enable1Pin, pwmChannel1);
    // configure LEDC PWM
    ledcAttachPin(enable2Pin, pwmChannel2);
}