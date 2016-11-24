#include <Arduino.h>

// pins
uint8_t serialData = 0;
uint8_t latch = 1;
uint8_t clk = 2;


int digits[4][7] = {{HIGH, HIGH, LOW,  LOW,  LOW,  LOW,  LOW}, // 1
                    {HIGH, LOW,  HIGH, HIGH, LOW,  HIGH, HIGH},// 2
                    {HIGH, HIGH, HIGH, HIGH, LOW,  HIGH, LOW}, // 3
                    {HIGH, HIGH, LOW,  HIGH, HIGH, LOW,  LOW}};// 4


volatile boolean colon = false;
volatile int display[4] = {1, 2, 3, 0};

void latchRegisters();

void initTimer1();

void pushRegister(int value);

int isr_digit = 0;
ISR(TIMER1_COMPA_vect) // Interrupt Service Routine
{
    for (int i = 0; i < 4; ++i) {
        pushRegister(isr_digit != i);
    }
    if (++isr_digit == 4) {
        isr_digit = 0;
    }

    pushRegister(colon);

    for (int i = 0; i < 7; i++) {
        pushRegister((uint8_t) digits[display[isr_digit]][i]);
    }
    latchRegisters();
}

void pushRegister(int value) {
    digitalWrite(serialData, (uint8_t) value);
    digitalWrite(clk, HIGH);
    digitalWrite(clk, LOW);
}

void latchRegisters() {
    digitalWrite(latch, HIGH);
    digitalWrite(latch, LOW);
}

void setup() {
    initTimer1();
    pinMode(serialData, OUTPUT);
    pinMode(latch, OUTPUT);
    pinMode(clk, OUTPUT);
}

void initTimer1() {
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

void loop() {
    delay(1000);
    colon = !colon;


}