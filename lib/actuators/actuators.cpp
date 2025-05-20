#include <actuators.h>
#include <debug.h>
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
#define maxMotorDir 200
#define maxMotorEsq 200


uint8_t resolution = 8;
uint32_t freq = 30000;
float base_speed = 180; // Velocidade padrão dos motores
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
    for(int posDegrees = servo1.read(); posDegrees >= rightAngle; posDegrees--) { // Mover o servo motor para a direita
        servo1.write(posDegrees);
        delay(servoDelay);
    }
    dist.setDistances(2); // Obter a distância lida pelo sensor de distância e salvá-la no índice 2 da matriz de distâncias
}

void Vision::eyeCenter(DISTSensor& dist){
    if(servo1.read() > straightAngle){ // Detectar se o motor está apontando para a direita ou esquerda
        for(int posDegrees = servo1.read(); posDegrees >= straightAngle; posDegrees--) { // Mover o servo motor para o centro
            servo1.write(posDegrees);
            delay(servoDelay);
    }
    }else if(servo1.read() < straightAngle){ // Detectar se o motor está apontando para a direita ou esquerda
        for(int posDegrees = servo1.read(); posDegrees <= straightAngle; posDegrees++) { // Mover o servo motor para o centro
            servo1.write(posDegrees);
            delay(servoDelay);
        }
    }
    dist.setDistances(1); // Obter a distância lida pelo sensor de distância e salvá-la no índice 1 da matriz de distâncias
}

void Vision::reading(DISTSensor& dist){ // Leitura de valores de distância à esqueda, centro e direita, preenchendo os índices do array
    eyeRight(dist);
    eyeLeft(dist);
    eyeCenter(dist);    
}

u_int16_t* Vision::getDistances(DISTSensor& dist){ // Obtenção do array distance[]
    return dist.getDistances(); // Retornar as distâncias presentes no objeto dist
}

Car::Direction Car::decideDirection(u_int16_t* distances){
    int smallestIndex = 0; // Variável para comparação de valores do array
    DEBUG_PRINT("--------------------------------------DECIDE DIRECITONS-----------------------------------");
    // Encontrar o menor valor no array de distâncias
    for (int i = 1; i < 3; i++) {
        DEBUG_PRINTLN("Distancias");
        DEBUG_PRINTLN(distances[i]);
        if (distances[i] < distances[smallestIndex]) {
            // DEBUG_PRINTLN("Distância lida:" + String(distances[i] + "na posição" + String(i)));
            smallestIndex = i;
        }
    }

    // Decidir para onde virar
    if (distances[smallestIndex] <= 500) {
        DEBUG_PRINTLN("A menor Distância lida:" + String(distances[smallestIndex]));
        if (smallestIndex == 0) return right; // O valor lido quando o sensor está na esquerda (eyeLeft) é guardado no índice 0
        if (smallestIndex == 2) return left; // O valor lido quando o sensor está na direita (eyeRight) é guardado no índice 2
    }
    return straight; // Padrão se não estiver perto de nada
}

float Car::calErr(u_int16_t* distances){
    float max_dist = 2000.0; // Distância máxima que o sensor detecta com certa precisão
    float norm_ctr = distances[1] / max_dist;
    // if (norm_ctr < 0.2) {
    //     return 0.0;    
    // }
    float norm_esq = distances[0]/max_dist; // Normalização dos valores de distância (valores entre 0 e 1)
    if (norm_esq > 1.0) norm_esq = 1.0; // Limitação do valor normalizado a 1 (caso o sensor acabe detectando algo a mais de 2 metros (2000 mm))
    DEBUG_PRINT("esque: ");
    DEBUG_PRINTLN(norm_esq);
    float norm_dir = distances[2]/max_dist;
    if (norm_dir > 1.0) norm_dir = 1.0;
    DEBUG_PRINT("dir: ");
    DEBUG_PRINTLN(norm_dir);
    float erro = norm_esq-norm_dir; // Calculo do erro, levando em consideração a diferença entre as detecções de distância à esquerda e à direita
    DEBUG_PRINT("Erro: ");
    DEBUG_PRINTLN(erro);
    return erro;
}

void Car::forward(float erro){
    DEBUG_PRINT ("Erro no forwared;");
    DEBUG_PRINT (erro); 
    DEBUG_PRINTLN();
    float ajuste = erro * 100.0; // Valor de erro ajustado para a magnitude da velocidade (*100)
    DEBUG_PRINT("Ajuste: ");
    DEBUG_PRINTLN(ajuste);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    int motor_esq = base_speed - ajuste; // Caso o valor de "ajuste" seja negativo, aumenta a velocidade do motor esquerdo (base_speed - (-ajuste))
    int motor_dir = base_speed + ajuste; // Caso o valor de "ajuste" seja positivo, aumenta a velocidade do motor direito (base_speed - ajuste)
    if(motor_esq < 0){ 
        digitalWrite(motor1Pin1, HIGH);// Alterando o sentido de giro dos motores caso o valor seja negativo
        digitalWrite(motor1Pin2, LOW); 
        motor_esq = -motor_esq; // Fazendo com que o valor de velocidade volte a ser positivo
    }else{
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
    }
    if(motor_dir < 0){
        digitalWrite(motor2Pin1, HIGH);
        digitalWrite(motor2Pin2, LOW);
        motor_dir = -motor_dir;

    }else{
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH);
        
    }
    if (motor_esq > maxMotorEsq) motor_esq = maxMotorEsq; // Limitando a velocidade máxima dos motores, evitando consumo elevado de corrente
    if (motor_dir > maxMotorDir) motor_dir = maxMotorDir;
    DEBUG_PRINT("motor_esq: ");
    DEBUG_PRINT(motor_esq);
    DEBUG_PRINTLN();
    DEBUG_PRINT("motor_dir: ");
    DEBUG_PRINT(motor_dir);
    DEBUG_PRINTLN();
    ledcWrite(pwmChannel1, motor_esq); // Acionamento dos motores utilizando os valores calculados anteriormente como duty cycle
    ledcWrite(pwmChannel2, motor_dir);
    return;
}

void Car::stop(){
    // Função utilizada para parar o carrinho, zerando o duty cycle dos motores e desabilitando os pinos de enable
    ledcWrite(pwmChannel1, 0);
    ledcWrite(pwmChannel2, 0);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}

// Métodos utilizados para fazer o carrinho virar completamente em alguma direção ou andar para frente (obsoletas na versão atual do carrinho, que utiliza apenas o método forward para se mover)
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
    u_int16_t now = millis(); // Início do timer
    while (millis() - now < 20000){ // Timer para que o carrinho se movimente por 20 segundos
        float err; // Variável utilizada para guardar o "erro" (diferença entre distância de obstáculos no lado esquerdo e direito do carro)
        vis.reading(dist); // Método utilizado para avaliar as distâncias de obstáculos à frente doc carrinho
        u_int16_t* distances = vis.getDistances(dist); // Obtenção dos valores de distância lidos pelo sensor na etapa anterior
        err = calErr(distances); // Cálculo do erro (diferença entre as distâncias na direita e na esquerda do carrinho)
        DEBUG_PRINT("err: ");
        
        if(err == 0.0){
            forward(err);
        }else{
            DEBUG_PRINTLN(err);
            forward(err); // Chamada da função "forward", responsável por movimentar o carrinho para frente controlando o duty cycle do PWM de cada motor para alterar a direção do carrinho
        }        
    }
    stop(); // Após o tempo determinado, parar o carrinho
}

void Car::begin(){
    DEBUG_PRINTLN("Car initialized");
    vis.begin(); // Inicializando o movimento do sensor de distância via servo motor
    pinMode(motor1Pin1, OUTPUT); // Configurando os pinos de enable como output
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(enable1Pin, OUTPUT);
    pinMode(enable2Pin, OUTPUT);

    ledcSetup(pwmChannel1, freq, resolution); // Configurando os canais para controle do sinal PWM (velocidade dos motores)
    ledcSetup(pwmChannel2, freq, resolution);
    
    // configuração LEDC PWM
    ledcAttachPin(enable1Pin, pwmChannel1);
    // configuração LEDC PWM
    ledcAttachPin(enable2Pin, pwmChannel2);
}