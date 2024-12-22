#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

class DISTSensor{
    private:
        u_int16_t distances[3] = {0,0,0};        
        Adafruit_VL53L0X lox = Adafruit_VL53L0X();


    public:
        u_int16_t len = sizeof(distances)/sizeof(distances[0]);
        void begin();
        bool measureClosing();
        uint16_t getDistance();
        uint16_t* getDistances();
        void setDistances(u_int8_t pos);
        bool measure();
};