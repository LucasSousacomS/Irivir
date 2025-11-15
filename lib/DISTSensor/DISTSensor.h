#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

class DISTSensor{
    private:
        VL53L0X_RangingMeasurementData_t measure;
        u_int16_t distances[3];        
        Adafruit_VL53L0X lox = Adafruit_VL53L0X();


    public:
        u_int16_t len = sizeof(distances)/sizeof(distances[0]);
        void begin();
        bool measureClosing();
        uint16_t getDistance();
        uint16_t* getDistances();
        void setDistances(u_int8_t pos);
        bool perfMeasure();
};