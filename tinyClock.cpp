#include <Arduino.h>
#include "TimeDisplay.h"

// pins
uint8_t serialData = 0;
uint8_t latch = 1;
uint8_t clk = 2;

TimeDisplay display(clk, latch, serialData);

ISR(TIMER1_COMPA_vect) // Interrupt Service Routine
{
    display.timerInterruptHandler();
}

void setup() {
    display.begin();
}

void loop() {
    delay(500);
    display.colonOn();
    delay(500);
    display.colonOff();
}