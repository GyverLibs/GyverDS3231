This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/GyverDS3231.svg?color=brightgreen)](https://github.com/GyverLibs/GyverDS3231/releases/latest/download/GyverDS3231.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/GyverDS3231.svg)](https://registry.platformio.org/libraries/gyverlibs/GyverDS3231)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverDS3231?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# GyverDS3231
Real-time chip (RTC) library DS3231
- Time is counted on the basis of millis() and synchronized with RTC by timer.
- Built-in second timer for easy process automation
- Works at the library.[Stamp](https://github.com/GyverLibs/Stamp)
- Ability to use an external Wire object

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

### Dependencies
- [Stamp](https://github.com/GyverLibs/Stamp) v1.5.0+

## Contents
- [Use of use](#usage)
- [Versions](#versions)
- [Installation](#install)
- [Bugs and feedback](#feedback)

<a id="usage"></a>

## Use of use
### GyverDS3231Min
Working directly with RTC and its time:

```cpp
// launch. You can specify a different Wire driver and i2c address. It will return true when successful.
bool begin(TwoWire* wire = &Wire, uint8_t addr = 0x68);

// RTC is working correctly
bool isOK();

// was feeding off
bool isReset();

// read out
Datime getTime();

// set the RTC time:
bool setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

// set the RTC time: ("hh:mm:ss") / ("yyyy-mm-dd") / ("yyyy-mm-ddThh:mm:ss") / (unix)
bool setTime(Datime dt);

// set the RTC time equal to the firmware compilation time
bool setBuildTime();

// get the temperature of the float chip
float getTemp();

// get the temperature of the chip int
int getTempInt();

// receive calibration offset (value * 0.1ppm)
int8_t getOffset();

// set calibration offset (value * 0.1ppm)
bool setOffset(int8_t offset);

// VirtualRTC
// set unix RTC time (to be brought to local)
void setUnix(uint32_t unix);

// Get unix RTC time (GMT+0)
uint32_t getUnix();
```

### GyverDS3231
The library counts time using millis() and synchronizes time with RTC over timer. Inherit.`GyverDS3231Min`and`StampKeeper`:

```cpp
// set the synchronization period in seconds
GyverDS3231(uint32_t syncPrd = 60 * 60);

// set the synchronization period in seconds
void setPeriod(uint32_t syncPrd);

// launch and sync. You can specify a different Wire driver and i2c address. It will return true when successful.
bool begin(TwoWire* wire = &Wire, uint8_t addr = 0x68);

// synchronize time with RTC. True at success, false at tire error or after RTC power reset
bool updateNow();

// get current unix from synchronized StampKeeper
uint32_t getUnix() const;

// ticker, call the loop. Returns true every second if time is synchronized
bool tick();
```

## Examples
### GyverDS3231Min

```cpp
#include <GyverDS3231Min.h>

GyverDS3231Min rtc;

void setup() {
    Serial.begin(115200);
    Serial.println("start");

    setStampZone(3);  // time-zone

    Wire.begin();
    rtc.begin();

    Serial.print("OK: ");
    Serial.println(rtc.isOK());

    Serial.print("Reset: ");
    Serial.println(rtc.isReset());

    // RTC was powered down, timing was incorrect.
    if (rtc.isReset()) {
        rtc.setBuildTime();     // set the firmware compilation time
        // rtc.setTime(2025, 1, 30, 12, 45, 0); // set the time manually
    }

    // put out
    Datime dt = rtc.getTime();
    Serial.println(dt.year);
    Serial.println(dt.month);
    Serial.println(dt.day);
    Serial.println(dt.hour);
    Serial.println(dt.minute);
    Serial.println(dt.second);
}

void loop() {
    // read out
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

    setStampZone(3);  // time-zone

    Wire.begin();
    rtc.begin();

    Serial.print("OK: ");
    Serial.println(rtc.isOK());

    Serial.print("Reset: ");
    Serial.println(rtc.isReset());

    // RTC was powered down, timing was incorrect.
    if (rtc.isReset()) {
        rtc.setBuildTime();     // set the firmware compilation time
        // rtc.setTime(2025, 1, 30, 12, 45, 0); // set the time manually
    }
}
void loop() {
    // The ticker will return true every second in 0 ms of a second if the time is synchronized.
    if (rtc.tick()) {
        // lineage
        Serial.print(rtc.toString());  // rtc.timeToString(), rtc.dateToString()
        Serial.print(':');
        Serial.println(rtc.ms());  // + milliseconds of the current second. Inside, the tick is always 0.

        // deduction
        Datime dt = rtc;  // or Datime dt(rtc)
        Serial.println(dt.year);
        Serial.println(dt.second);
        Serial.println(dt.hour);
        dt.weekDay();
        dt.yearDay();
        // ... and other Datime methods and variables

        // Reading directly slower than output in Datime
        rtc.second();
        rtc.minute();
        rtc.year();
        // ... and other methods of StampConvert

        // comparison
        rtc == DaySeconds(12, 35, 0);            // Compared to DaySeconds (time is 12:35:00)
        rtc == 1738237474ul;                     // comparison with unix
        rtc == Datime(2025, 1, 30, 14, 14, 30);  // comparison
    }

    if (rtc.newSecond()) {
        // You can catch another second here.
    }
}
```

<a id="versions"></a>

## Versions
- v1.0
- v1.1.0 – divided into two classes, now RTC runs on the StampKeeper API

<a id="install"></a>

## Installation
- The library can be found under the name **GyverDS3231** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/GyverDS3231/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="feedback"></a>

## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
