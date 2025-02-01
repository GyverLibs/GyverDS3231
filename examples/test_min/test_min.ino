#include <GyverDS3231.h>

GyverDS3231Min rtc;

void setup() {
    Serial.begin(115200);
    Serial.println("start");

    setStampZone(3);  // часовой пояс

    Wire.begin();
    rtc.begin();

    Serial.print("OK: ");
    Serial.println(rtc.isOK());

    Serial.print("Reset: ");
    Serial.println(rtc.isReset());

    // был сброс питания RTC, время некорректное
    if (rtc.isReset()) {
        rtc.setBuildTime();     // установить время компиляции прошивки
        // rtc.setTime(2025, 1, 30, 12, 45, 0); // установить время вручную
    }

    // вывести в Datime
    Datime dt = rtc.getTime();
    Serial.println(dt.year);
    Serial.println(dt.month);
    Serial.println(dt.day);
    Serial.println(dt.hour);
    Serial.println(dt.minute);
    Serial.println(dt.second);
}

void loop() {
    // прочитать и вывести время строкой
    Serial.println(rtc.getTime().toString());
    delay(1000);
}