#include "DHT22.h"

DHT22::DHT22(uint8_t pinSDA) {
    this->pinSDA = pinSDA;
};

void DHT22::read() {
    uint8_t lastState = HIGH;
    uint8_t counter = 0;
    uint8_t vi = 0;
    uint8_t vals[5] = {0, 0, 0, 0, 1};

    pinMode(pinSDA, OUTPUT);
    digitalWrite(pinSDA, HIGH);

    if (firstrun) {
        firstrun = false;

        delay(1000);
    } else {
        delay(80);
    }

    pinMode(pinSDA, OUTPUT);
    digitalWrite(pinSDA, LOW);
    delay(20);
    noInterrupts();
    digitalWrite(pinSDA, HIGH);
    delayMicroseconds(40);
    pinMode(pinSDA, INPUT);

    for (uint8_t i = 0; i < 85; i++) {
        counter = 0;
        
        while(digitalRead(pinSDA) == lastState) {
            if (counter == 255) break;
            
            counter += 1;
            delayMicroseconds(1);
        }

        if (counter == 255) break;

        lastState = digitalRead(pinSDA);

        if (i < 4 || i % 2 != 0) continue;

        vals[vi / 8] <<= 1;

        if (counter > 6) {
            vals[vi / 8] |= 1;
        }

        vi += 1;
    }

    interrupts();

    /*
    Serial.print(vals[0], HEX); Serial.print(", ");
    Serial.print(vals[1], HEX); Serial.print(", ");
    Serial.print(vals[2], HEX); Serial.print(", ");
    Serial.print(vals[3], HEX); Serial.print(", ");
    Serial.print(vals[4], HEX); Serial.print("\n");
    Serial.println(vals[0] + vals[1] + vals[2] + vals[3], HEX);
    */

    if (vals[4] != ((vals[0] + vals[1] + vals[2] + vals[3]) & 0xFF)) {
        firstrun = true;
        humidity = NAN;
        temperature = NAN;
    } else {
        humidity = (float(vals[0]) * float(256) + float(vals[1])) / 10;
        temperature = (float(vals[2] & 0x7F) * float(256) + float(vals[3])) / 10;

        if (vals[2] & 0x80) temperature *= -1;
    }
}
