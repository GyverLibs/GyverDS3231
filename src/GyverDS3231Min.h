#pragma once
#include <Arduino.h>
#include <Datime.h>
#include <StampBuild.h>
#include <VirtualRTC.h>
#include <Wire.h>

#define DS_SEC_ADDR 0x00
#define DS_OFFS_ADDR 0x10
#define DS_TEMP_ADDR 0x11

class GyverDS3231Min : public VirtualRTC {
   public:
    // запустить. Можно указать другой драйвер Wire и i2c адрес. Вернёт true при успехе
    bool begin(TwoWire* wire = &Wire, uint8_t addr = 0x68) {
        _wire = wire;
        _addr = addr;
        return isOK();
    }

    // RTC работает корректно
    bool isOK() {
        return getTime().valid();
    }

    // был сброс питания
    bool isReset() {
        return getTime().year == 2000;
    }

    // прочитать время RTC
    Datime getTime() {
        Datime dt(0, 0, 0, 0, 0, 0);
        if (!_wire) return dt;

        _wire->beginTransmission(_addr);
        _wire->write(DS_SEC_ADDR);
        if (_wire->endTransmission()) return dt;

        uint8_t n = _wire->requestFrom(_addr, (uint8_t)7);
        if (n != 7) return dt;

        dt.second = _decodeReg(_wire->read());
        dt.minute = _decodeReg(_wire->read());
        dt.hour = _decodeHours(_wire->read());
        _wire->read();  // weekday
        dt.day = _decodeReg(_wire->read());
        dt.month = _decodeReg(_wire->read());
        dt.year = _decodeReg(_wire->read()) + 2000;
        dt.updateDays();
        return dt;
    }

    // установить время RTC:
    bool setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
        return setTime(Datime(year, month, day, hour, minute, second));
    }

    // установить время RTC: ("hh:mm:ss") / ("yyyy-mm-dd") / ("yyyy-mm-ddThh:mm:ss") / (unix)
    bool setTime(Datime dt) {
        if (!_wire || !dt.valid() || dt.isY2K()) return 0;

        dt.updateDays();
        _wire->beginTransmission(_addr);
        _wire->write(DS_SEC_ADDR);
        _wire->write(_encodeReg(dt.second));
        _wire->write(_encodeReg(dt.minute));
        if (dt.hour > 19) _wire->write((0x2 << 4) | (dt.hour % 20));
        else if (dt.hour > 9) _wire->write((0x1 << 4) | (dt.hour % 10));
        else Wire.write(dt.hour);
        _wire->write(dt.weekDay);
        _wire->write(_encodeReg(dt.day));
        _wire->write(_encodeReg(dt.month));
        _wire->write(_encodeReg(dt.year - 2000));
        bool end = !_wire->endTransmission();

        if (end) updateNow();
        return end;
    }

    // установить время RTC равным времени компиляции прошивки
    bool setBuildTime() {
        return setTime(__DATE_YEAR__, __DATE_MONTH__, __DATE_DAY__, __TIME_HOUR__, __TIME_MIN__, __TIME_SEC__);
    }

    // получить температуру чипа float
    float getTemp() {
        return (_getTempRaw() * 0.25f);
    }

    // получить температуру чипа int
    int getTempInt() {
        return (_getTempRaw() >> 2);
    }

    // получить смещение калибровки (значение * 0.1ppm)
    int8_t getOffset() {
        if (!_wire) return 0;

        _wire->beginTransmission(_addr);
        _wire->write(DS_OFFS_ADDR);
        if (_wire->endTransmission()) return 0;

        _wire->requestFrom(_addr, (uint8_t)1);
        return _wire->read();
    }

    // установить смещение калибровки (значение * 0.1ppm)
    bool setOffset(int8_t offset) {
        if (!_wire) return 0;

        _wire->beginTransmission(_addr);
        _wire->write(DS_OFFS_ADDR);
        _wire->write(offset);
        return !_wire->endTransmission();
    }

    // установить unix время RTC (будет приведено к локальному)
    void setUnix(uint32_t unix) {
        setTime(unix);
    }

    // получить unix время RTC (GMT+0)
    uint32_t getUnix() {
        Datime dt = getTime();
        return (dt.valid() && dt.year != 2000) ? dt.getUnix() : 0;
    }

   protected:
    virtual bool updateNow() { return 1; }

   private:
    TwoWire* _wire = nullptr;
    uint8_t _addr;

    uint8_t _encodeReg(int8_t data) {
        return (((data / 10) << 4) | (data % 10));
    }
    uint8_t _decodeReg(uint8_t data) {
        return ((data >> 4) * 10 + (data & 0xF));
    }
    uint8_t _decodeHours(uint8_t data) {
        return (data & 0xF) + ((data & 0x20) ? 20 : ((data & 0x10) ? 10 : 0));
    }
    int _getTempRaw() {
        if (!_wire) return 0;

        _wire->beginTransmission(_addr);
        _wire->write(DS_TEMP_ADDR);
        if (_wire->endTransmission()) return 0;

        _wire->requestFrom(_addr, (uint8_t)2);
        return ((int8_t)_wire->read() << 2) + (_wire->read() >> 6);
    }
};