#include "Arduino.h"

class DHT22 {
    private:
        uint8_t pinSDA;

        bool firstrun = true;

    public:
        DHT22(uint8_t pinSDA);

        float humidity = NAN;
        float temperature = NAN;

        void read();
};