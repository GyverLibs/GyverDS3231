#include <Arduino.h>
#include <GyverDS3231.h>
GyverDS3231 ds;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    if (!ds.begin()) ds.setBuildTime();
}

void loop() {
    if (ds.tick()) {
        Serial.println(ds.toString());

        Datime dt(ds);
        Serial.println(dt.second);
        Serial.println(dt.minute);
        // ...
    }
}