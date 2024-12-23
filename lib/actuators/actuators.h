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

    void turn(Direction dir){ // Função para fazer o carrinho virar
        if(dir == left){ 
            Serial.print("Virar à esquerda");
        }else{
            Serial.print("Virar à direita");
        }
    }

    void forward(){ // Função para fazer o carrinho andar pra frente
        Serial.println("Frente");
    }

    void backward(){ // Função para fazer o carrinho andar pra trás
        Serial.println("Trás");
    }

    public:

    void mind(DISTSensor& dist){ // Função que faz o carrinho agir (recebendo a referência do ponteiro dist (&), para usar o mesmo objeto que foi criado no início do programa)
        u_int16_t now = millis();
        while (millis() - now < 10000){
            vis.reading(dist);
            u_int16_t* distances =  vis.getDistances(dist);
            Direction dir = decideDirection(distances);
            if(dir != straight) turn(dir);
            else forward();
        }        
    }
};
