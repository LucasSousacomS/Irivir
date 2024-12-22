#include <Arduino.h>
#include <DISTSensor.h>

class Vision{
    private:
        DISTSensor dist;
    public:
    void eyeLeft(){
        Serial.println("eyeLeft");
        delay(1500);
        dist.setDistances(0);
    }

    void eyeRight(){
        Serial.println("eyeRight");
        delay(1500);
        dist.setDistances(2);
    }

    void eyeCenter(){
        Serial.println("eyeCenter");
        delay(1500);
        dist.setDistances(1);
    }

    void reading(){
        eyeLeft();
        eyeCenter();
        eyeRight();
        eyeCenter();       
    }

    u_int16_t* getDistances(){
        return dist.getDistances();
    }

};

class Car{
    private:
    enum Direction: bool{left = false, right = true};
    Vision vis;
    Direction decideDirection(uint16_t* distances) {
        int smallestIndex = 0;

        // Encontrar o menor valor no array
        for (int i = 1; i < 3; i++) {
            if (distances[i] < distances[smallestIndex]) {
                smallestIndex = i;
            }
        }

        // Decidir para onde virar
        if (distances[smallestIndex] <= 100) {
            if (smallestIndex == 0) return right;
            if (smallestIndex == 2) return left;
        }
        return left; // Padrão se não estiver perto de nada
    }

    void turn(Direction dir){
        if(dir == left){
            Serial.print("Virar à esquerda");
        }else{
            Serial.print("Virar à direita");
        }
    }

    public:

    void mind(){
        vis.reading();
        u_int16_t* distances =  vis.getDistances();
        Direction dir = decideDirection(distances);
        turn(dir);
    }
};
