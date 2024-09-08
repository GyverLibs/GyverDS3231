#pragma once
#include <Arduino.h>
#include <Stamp.h>
#include <Wire.h>

#define DS_SEC_ADDR 0x00
#define DS_OFFS_ADDR 0x10
#define DS_TEMP_ADDR 0x11

class GyverDS3231 : public StampTicker {
   public:
    // передать период синхронизации в секундах
    GyverDS3231(uint16_t syncPrd = 60 * 60) : _prd(syncPrd * 1000ul) {}

    // запустить и синхронизировать. Можно указать другой драйвер Wire и i2c адрес. Вернёт true при успехе
    bool begin(TwoWire* wire = &Wire, uint8_t addr = 0x68) {
        _wire = wire;
        _addr = addr;
        return updateNow();
    }

    // RTC работает корректно (чтение и валидация времени)
    bool isOK() {
        return getTime().valid();
    }

    // синхронизировать время с RTC. true при успехе, false при ошибке шины или после сброса питания RTC
    bool updateNow() {
        Datime dt = getTime();
        if (!dt.valid() || dt.isY2K()) return 0;
        StampTicker::update(dt.getUnix());
        _tmr = millis();
        return 1;
    }

    // прочитать время с RTC
    Datime getTime() {
        Datime dt;
        if (!_wire) return dt;
        _wire->beginTransmission(_addr);
        _wire->write(DS_SEC_ADDR);
        if (_wire->endTransmission()) return dt;

        _wire->requestFrom(_addr, (uint8_t)7);
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
    // ("hh:mm:ss") или ("yyyy-mm-dd") или ("yyyy-mm-ddThh:mm:ss")
    // (unix)
    // Datime(year, month, day, hour, minute, second)
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

        if (!_wire->endTransmission()) {
            StampTicker::update(dt);
            _tmr = millis();
            return 1;
        }
        return 0;
    }

    // установить время RTC равным локальному времени компиляции прошивки
    bool setBuildTime() {
        return setTime(BUILD_STAMP_LOCAL);
    }

    // тикер, вызывать в loop. Возвращает true каждую секунду, если время синхронизировано
    bool tick() {
        if (synced() && millis() - _tmr >= _prd) updateNow();
        return StampTicker::tick();
    }

    // получить температуру чипа
    float getTemp() {
        return (_getTempRaw() * 0.25f);
    }

    // получить температуру чипа
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

   private:
    uint32_t _tmr = 0, _prd = 0;
    TwoWire* _wire = nullptr;
    uint8_t _addr;

    using StampTicker::update;

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