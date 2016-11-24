//
// Created by Sergey on 23/11/2016.
//
#include "TimeDisplay.h"
#include "Arduino.h"

uint8_t digits[4][7] = {{HIGH, HIGH, LOW,  LOW,  LOW,  LOW,  LOW}, // 1
                        {HIGH, LOW,  HIGH, HIGH, LOW,  HIGH, HIGH},// 2
                        {HIGH, HIGH, HIGH, HIGH, LOW,  HIGH, LOW}, // 3
                        {HIGH, HIGH, LOW,  HIGH, HIGH, LOW,  LOW}};// 4


uint8_t disp[4] = {1, 2, 3, 0};


TimeDisplay::TimeDisplay(uint8_t clock, uint8_t latch, uint8_t data) {
    _data_pin = data;
    _latch_pin = latch;
    _clock_pin = clock;
}

void TimeDisplay::begin() {
    pinMode(_data_pin, OUTPUT);
    pinMode(_latch_pin, OUTPUT);
    pinMode(_clock_pin, OUTPUT);
    initTimer1();
}

void TimeDisplay::initTimer1() {
    cli(); // Disable global interrupts

    TCCR1 |= ((1 << CS12) | (1 << CS10)); // Timer 1 prescaling - divides by 16
    TCCR1 &= ~(1 << CS11); // Timer 1 prescaling - divides by 16
    TCCR1 |= (1 << CTC1); // Put timer 1 in CTC mode
    // OCR0A = 100; // Count 100 cycles for interrupt
    OCR1A = 200; // Count 50 cycles for interrupt
    //OCR0A = 10; // Count 10 cycles for interrupt
    TIMSK |= (1 << OCIE1A); // enable timer compare interrupt

    DDRB |= (1 << PB0); // Set PortB Pin0 as an output
    PORTB |= (1 << PB0); // Set PortB Pin0 high to turn on LED

    sei(); // Enable global interrupts
}

/**
 * To be called from within ISR(TIMER1_COMPA_vect)
 */
void TimeDisplay::timerInterruptHandler() {
    static int isr_digit = 0;
    for (int i = 0; i < 4; ++i) {
        pushRegister(isr_digit != i);
    }
    if (++isr_digit == 4) {
        isr_digit = 0;
    }

    pushRegister(_colon);

    for (int i = 0; i < 7; i++) {
        pushRegister((uint8_t) digits[disp[isr_digit]][i]);
    }
    latchRegisters();
}

void TimeDisplay::pushRegister(int value) {
    digitalWrite(_data_pin, (uint8_t) value);
    digitalWrite(_clock_pin, HIGH);
    digitalWrite(_clock_pin, LOW);
}

void TimeDisplay::latchRegisters() {
    digitalWrite(_latch_pin, HIGH);
    digitalWrite(_latch_pin, LOW);
}

void TimeDisplay::colonOn() {
    _colon = true;
}

void TimeDisplay::colonOff() {
    _colon = false;
}
