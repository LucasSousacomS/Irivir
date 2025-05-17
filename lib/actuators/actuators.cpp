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

#define leftAngle 130
#define rightAngle 50
#define straightAngle 90


uint8_t resolution = 8;
uint32_t freq = 30000;
int dutyCycle = 200;
Servo servo1;

void Vision::begin(){
    servo1.attach(servoPin);
    servo1.write(90);
}

void Vision::eyeLeft(DISTSensor& dist){
    for(int posDegrees = servo1.read(); posDegrees <= leftAngle; posDegrees++) {
        servo1.write(posDegrees);
        delay(servoDelay);
    }
    dist.setDistances(0); // Setar o valor do índice 0 da variável distâncias
}

void Vision::eyeRight(DISTSensor& dist){
    for(int posDegrees = servo1.read(); posDegrees >= rightAngle; posDegrees--) {
        servo1.write(posDegrees);
        delay(servoDelay);
    }
    dist.setDistances(2);
}

void Vision::eyeCenter(DISTSensor& dist){
    if(servo1.read() > straightAngle){
        for(int posDegrees = servo1.read(); posDegrees >= straightAngle; posDegrees--) {
        servo1.write(posDegrees);
        delay(servoDelay);
    }
    }else if(servo1.read()<90){
        for(int posDegrees = servo1.read(); posDegrees <= straightAngle; posDegrees++) {
            servo1.write(posDegrees);
            delay(servoDelay);
        }
    }
    dist.setDistances(1);
}

void Vision::reading(DISTSensor& dist){ // Leitura de valores de distância à esqueda, centro e direita, preenchendo os índices do array
    // dist.begin(); // Precia chamar a função begin de novo pois estou usando um outro objeto para controlar o sensor agora (este dist não é o mesmo dist do main)
    eyeRight(dist);
    eyeLeft(dist);
    eyeCenter(dist);    
}

u_int16_t* Vision::getDistances(DISTSensor& dist){ // Obtenção do array distance[]
    return dist.getDistances();
}

Car::Direction Car::decideDirection(u_int16_t* distances){
    int smallestIndex = 0; // Variável para comparação de valores do array
    Serial.print("--------------------------------------DECIDE DIRECITONS-----------------------------------");
    // Encontrar o menor valor no array de distâncias
    for (int i = 1; i < 3; i++) {
        Serial.println("Distancias");
        Serial.println(distances[i]);
        if (distances[i] < distances[smallestIndex]) {
            // Serial.println("Distância lida:" + String(distances[i] + "na posição" + String(i)));
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

float calcErr(u_int16_t* distances){
    float max_dist = 500.0;
    float norm_esq = distances[0] / max_dist;
    float norm_ctr = distances[1] / max_dist;
    float norm_dir = distances[2] / max_dist;
    float erro = norm_esq-norm_dir;
    return erro;
}

float forward(float erro){
    float base_speed = 180;
    float ajuste = erro * 100; // sensibilidade
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    int motor_esq = base_speed - ajuste;
    int motor_dir = base_speed + ajuste;
    ledcWrite(pwmChannel1, motor_esq);
    ledcWrite(pwmChannel2, motor_dir);
}

void Car::turn(Direction dir){
    if(dir == left){ 
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH);
        if(lastDir != left){
            for(int i = 100; i<180; i++){
                ledcWrite(pwmChannel2, i);
                delay(10);
            }
        }
        lastDir = left;
    }else{
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW);
        if(lastDir != right){
            for(int i = 100; i<180; i++){
                ledcWrite(pwmChannel1, i);
                delay(10);
            }
        }
        lastDir = right;
    }
}

void Car::forwardSimp(){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    
    if(lastDir != straight){
        for(int i = 100; i<180; i++){
            ledcWrite(pwmChannel1, i);
            ledcWrite(pwmChannel2, i);
            delay(10);
        }
    }    
    lastDir = straight;
}

void Car::backward(){ // Função para fazer o carrinho andar pra trás
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    for(int i = 100; i<180; i++){
        ledcWrite(enable1Pin, i);
        ledcWrite(enable2Pin, i);
        delay(10);
    }
}

void Car::mind(DISTSensor& dist){
    u_int16_t now = millis();
    while (millis() - now < 10000){
        vis.reading(dist);
        u_int16_t* distances = vis.getDistances(dist);
        forward(calcErr(distances));
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