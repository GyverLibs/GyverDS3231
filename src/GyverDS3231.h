#pragma once
#include <GyverDS3231Min.h>
#include <StampKeeper.h>

class GyverDS3231 : public GyverDS3231Min, public StampKeeper {
   public:
    // установить период синхронизации в секундах
    GyverDS3231(uint16_t syncPrd = 60 * 60) {
        setPeriod(syncPrd);
    }

    // установить период синхронизации в секундах
    void setPeriod(uint16_t syncPrd) {
        _prd = syncPrd * 1000ul;
    }

    // запустить и синхронизировать. Можно указать другой драйвер Wire и i2c адрес. Вернёт true при успехе
    bool begin(TwoWire* wire = &Wire, uint8_t addr = 0x68) {
        if (GyverDS3231Min::begin(wire, addr)) return updateNow();
        return false;
    }

    // синхронизировать время с RTC. true при успехе, false при ошибке шины или после сброса питания RTC
    bool updateNow() {
        _tmr = millis();
        Datime dt = getTime();
        if (!dt.valid() || dt.year == 2000) return false;

        StampKeeper::sync(dt.getUnix());
        return true;
    }

    // тикер, вызывать в loop. Возвращает true каждую секунду, если время синхронизировано
    bool tick() {
        if (millis() - _tmr >= _prd) updateNow();
        return StampKeeper::tick();
    }

   private:
    uint32_t _tmr = 0, _prd = 0;
};