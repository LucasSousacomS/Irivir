#include <Arduino.h>
#include <DISTSensor.h>
#include <ESP32Servo.h>

class Vision{ // Classe do sensor de distância
    private:
        // DISTSensor dist;
    public:
    void begin();

    void eyeLeft(DISTSensor& dist); // Movimentar o sensor para a direita com um servo motor
    // O mesmo se repete para right e center
    void eyeRight(DISTSensor& dist);    

    void eyeCenter(DISTSensor& dist);

    void reading(DISTSensor& dist);

    u_int16_t* getDistances(DISTSensor& dist);

};

class Car{ // Classe de controle do carrinho
    private:
    enum Direction: u_int8_t{left = 0, right = 1, straight = 2}; // Configurando valores a serem considerados para direções do carrinho usando um enum
    Vision vis;
    Direction decideDirection(uint16_t* distances) { // Função que decide a direção que o carrinho irá se mover
        
    }

    void turn(Direction dir){ // Função para fazer o carrinho virar

    }

    void forward(){ // Função para fazer o carrinho andar pra frente
        
    }

    void backward(){ // Função para fazer o carrinho andar pra trás
        
    }

    public:

    void mind(DISTSensor& dist){ // Função que faz o carrinho agir (recebendo a referência do ponteiro dist (&), para usar o mesmo objeto que foi criado no início do programa)
              
    }
};
