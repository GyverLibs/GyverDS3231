#include <Arduino.h>
#include <GyverDS3231.h>

GyverDS3231 rtc;

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
}
void loop() {
    // тикер вернёт true каждую секунду в 0 мс секунды, если время синхронизировано
    if (rtc.tick()) {
        // вывод даты и времени строкой
        Serial.print(rtc.toString());  // rtc.timeToString(), rtc.dateToString()
        Serial.print(':');
        Serial.println(rtc.ms());  // + миллисекунды текущей секунды. Внутри tick всегда равно 0

        // вывод в Datime
        Datime dt = rtc;  // или Datime dt(rtc)
        dt.year;
        dt.second;
        dt.hour;
        dt.weekDay;
        dt.yearDay;
        // ... и прочие методы и переменные Datime

        // чтение напрямую, медленнее чем вывод в Datime
        rtc.second();
        rtc.minute();
        rtc.year();
        // ... и прочие методы StampConvert

        // сравнение
        rtc == DaySeconds(12, 35, 0);            // сравнение с DaySeconds (время равно 12:35:00)
        rtc == 1738237474;                       // сравнение с unix
        rtc == Datime(2025, 1, 30, 14, 14, 30);  // сравнение с Datime
    }

    if (rtc.newSecond()) {
        // новую секунду можно поймать и здесь
    }
}