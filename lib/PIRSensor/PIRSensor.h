#include <Arduino.h>

class PIRSensor{
    private:
        u_int8_t pin;
        u_int32_t timeNow;
    public:

        PIRSensor(u_int8_t pin);

        void begin();

        bool detection();
};