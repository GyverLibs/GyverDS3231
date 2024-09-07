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
- [Stamp](https://github.com/GyverLibs/Stamp)

## Содержание
- [Использование](#usage)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="usage"></a>

## Использование
Библиотека считает время при помощи millis() и синхронизирует время с RTC по таймеру (по умолчанию каждый час).

```cpp
// наследует StampTicker

// передать период синхронизации в секундах, умолч. 1 час
GyverDS3231(uint16_t syncPrd = 60 * 60);

// запустить и синхронизировать. Можно указать другой драйвер Wire и i2c адрес. Вернёт true при успехе
bool begin(TwoWire* wire = &Wire, uint8_t addr = 0x68);

// RTC работает корректно (чтение и валидация времени)
bool isOK();

// синхронизировать время с RTC. true при успехе, false при ошибке шины или после сброса питания RTC
bool updateNow();

// прочитать время с RTC для отладки
Datime getTime();

// установить время RTC равным локальному времени компиляции прошивки
bool setBuildTime();

// установить время RTC:
// ("hh:mm:ss") или ("yyyy-mm-dd") или ("yyyy-mm-ddThh:mm:ss")
// (unix)
// Datime(year, month, day, hour, minute, second)
bool setTime(Datime dt);

// тикер, вызывать в loop. Возвращает true каждую секунду, если время синхронизировано
bool tick();

// получить температуру чипа
float getTemp();

// получить температуру чипа
int getTempInt();

// получить смещение калибровки (значение * 0.1ppm)
int8_t getOffset();

// установить смещение калибровки (значение * 0.1ppm)
bool setOffset(int8_t offset);
```

### Примеры
```cpp
#include <GyverDS3231.h>
GyverDS3231 ds;

void setup() {
    setStampZone(3);  // указать часовой пояс, если в программе нужен реальный unix
    // если не нужен - можно не указывать (останется 0)

    Serial.begin(115200);
    Wire.begin();

    // запустить и синхронизировать время, если возможно
    ds.begin();

    // запустить на другом драйвере I2C
    // ds.begin(&customWire);

    // ИЛИ
    // если старт неудачный (например был сброс питания RTC)
    // установить время равным времении компиляции
    // if (!ds.begin()) ds.setBuildTime();

    // установка времени вручную
    // ds.setTime(123456789ul);                     // unix
    // ds.setTime(Datime(2024, 7, 27, 17, 6, 30));  // Y.M.D H:M:S
    
    // Datime dt;
    // dt.second = 34;
    // ...задать дату и время...
    // и отправить в RTC
    // ds.setTime(dt);
}

void loop() {
    // вызывать тикер в loop
    // тикер также вернёт true каждую секунду, что удобно для сравнения времени
    if (ds.tick()) {
        Serial.println(ds.toString());     // вывод даты и времени строкой
        Serial.println(ds.dateToString()); // вывод даты строкой

        // можно сравнивать напрямую с unix
        if (ds >= 12345) { }
        if (ds == 123456) { }

        ds.getUnix();  // получить unix секунды

        // парсинг unix на дату и время
        ds.second();   // секунды
        ds.minute();   // минуты и так далее

        // эффективнее использовать парсер Datime
        Datime dt(ds);  // ds само конвертируется в Datime

        dt.year;
        dt.month;
        dt.day;
        dt.hour;
        dt.minute;
        dt.second;
        dt.weekDay;
        dt.yearDay;

        // для автоматизации внутри суток удобно использовать 
        // секунды с начала суток, daySeconds()
        ds.daySeconds();

        // для удобства также есть класс DaySeconds, позволяющий задать время внутри суток
        DaySeconds dsec(5, 10, 0);  // 5 часов, 10 минут, 0 секунд

        // GyverDS3231 может сравниваться напрямую с DaySeconds
        if (ds == dsec) { }
    }
}
```

<a id="versions"></a>

## Версии
- v1.0

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