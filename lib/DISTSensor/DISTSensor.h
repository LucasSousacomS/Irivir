#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

class DISTSensor{
    private:
        u_int16_t distance;
        Adafruit_VL53L0X lox = Adafruit_VL53L0X();

    public:
        void begin();
        bool measureClosing();
        uint16_t getDistance();
        bool measure();
};