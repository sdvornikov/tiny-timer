//
// Created by Sergey on 23/11/2016.
//

#ifndef ARDUINOCLION_TIMEDISPLAY_H
#define ARDUINOCLION_TIMEDISPLAY_H


#include <stdint.h>

class TimeDisplay {
public:
    TimeDisplay(uint8_t clock, uint8_t latch, uint8_t data);

    void begin();

    void colonOn();

    void colonOff();

    void timerInterruptHandler();

private:
    void initTimer1();

    void latchRegisters();

    void pushRegister(int value);

    uint8_t _clock_pin;
    uint8_t _latch_pin;
    uint8_t _data_pin;

    bool _colon = false;

};


#endif //ARDUINOCLION_TIMEDISPLAY_H
