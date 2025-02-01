[![latest](https://img.shields.io/github/v/release/GyverLibs/GyverDS3231.svg?color=brightgreen)](https://github.com/GyverLibs/GyverDS3231/releases/latest/download/GyverDS3231.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/GyverDS3231.svg)](https://registry.platformio.org/libraries/gyverlibs/GyverDS3231)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverDS3231?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# GyverDS3231
Библиотека для работы с микросхемой реального времени (RTC) DS3231
- Время считается на базе millis() и синхронизируется с RTC по таймеру
- Встроенный секундный таймер для удобства автоматизации процессов
- Работает на базе библиотеки [Stamp](https://github.com/GyverLibs/Stamp)
- Возможность использовать внешний объект Wire

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

### Зависимости
- [Stamp](https://github.com/GyverLibs/Stamp) v1.4.0+

## Содержание
- [Использование](#usage)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="usage"></a>

## Использование
### GyverDS3231Min
Работа напрямую с RTC и его временем:

```cpp
// запустить. Можно указать другой драйвер Wire и i2c адрес. Вернёт true при успехе
bool begin(TwoWire* wire = &Wire, uint8_t addr = 0x68);

// RTC работает корректно
bool isOK();

// был сброс питания
bool isReset();

// прочитать время RTC
Datime getTime();

// установить время RTC:
bool setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

// установить время RTC: ("hh:mm:ss") / ("yyyy-mm-dd") / ("yyyy-mm-ddThh:mm:ss") / (unix)
bool setTime(Datime dt);

// установить время RTC равным времени компиляции прошивки
bool setBuildTime();

// получить температуру чипа float
float getTemp();

// получить температуру чипа int
int getTempInt();

// получить смещение калибровки (значение * 0.1ppm)
int8_t getOffset();

// установить смещение калибровки (значение * 0.1ppm)
bool setOffset(int8_t offset);

// VirtualRTC
// установить unix время RTC (будет приведено к локальному)
void setUnix(uint32_t unix);

// получить unix время RTC (GMT+0)
uint32_t getUnix();
```

### GyverDS3231
Библиотека считает время при помощи millis() и синхронизирует время с RTC по таймеру. Наследует `GyverDS3231Min` и `StampKeeper`:

```cpp
// установить период синхронизации в секундах
GyverDS3231(uint16_t syncPrd = 60 * 60);

// установить период синхронизации в секундах
void setPeriod(uint16_t syncPrd);

// запустить и синхронизировать. Можно указать другой драйвер Wire и i2c адрес. Вернёт true при успехе
bool begin(TwoWire* wire = &Wire, uint8_t addr = 0x68);

// синхронизировать время с RTC. true при успехе, false при ошибке шины или после сброса питания RTC
bool updateNow();

// тикер, вызывать в loop. Возвращает true каждую секунду, если время синхронизировано
bool tick();
```

## Примеры
### GyverDS3231Min

```cpp
#include <GyverDS3231Min.h>

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
```

### GyverDS3231

```cpp
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
```

<a id="versions"></a>

## Версии
- v1.0
- v1.1.0 - разделено на два класса, теперь RTC работает на API StampKeeper

<a id="install"></a>

## Установка
- Библиотеку можно найти по названию **GyverDS3231** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GyverDS3231/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="feedback"></a>

## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код