// INCLUDE LIBRARIES
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart_header.h"

// CONFIGURATION BITS
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)

#define _XTAL_FREQ 4000000      // Compiler reference

// VARIABLES
int adc_raw;
unsigned char str[8];
volatile unsigned char num = 0;

// FUNCTIONS
// Function initialize the ADC moduel
void adc_init(void) {
    ADCON0bits.ADFM     = 1;        // Right justify
    ADCON0bits.VCFG     = 0;        // VDD as reference
    ADCON0bits.CHS      = 0b111;    // Analog channel AN7
    ADCON0bits.ADON     = 1;        // ADC enable
    ADCON1bits.ADCS     = 0b010;    // ADC conversion set bit
}

// Function read the ADC values
void adc_read(void) {
    ADCON0bits.GO       = 1;            // Start ADC
    __delay_us(10);
    while (ADCON0bits.nDONE);           // Wait for ADC to finish
    adc_raw = ADRESL + (ADRESH * 256);
        itoa(str, adc_raw, 10);
        uart_send_str(str);
        uart_send_str("\r\n");
}

// The interrupt function
/*
 * 0.25 sec
 * 2500000 / 256 = 976 -> 9 preload tmr0
 * 256 - 9 = 247
 * 976 / 247 = 3.95 number of overflows aka num

 * 0.125 sec
 * 125000 / 256 = 488 -> 4 preload tmr0
 * 256 - 4 = 252
 * 488 / 252 = 1.94 number of overflows aka num
 */

void __interrupt() ISR(void) {
   num++;
    if (num == 2) {
        adc_read();
        num = 0;
    }
    TMR0 = 4;
    INTCONbits.T0IF = 0;
}

// Main program
void main(void) {
    CMCON0      = 0x07;             // Turn comparato off
    TRISC       = 0b111000;         // RX -TX - AN7 as input
    PORTC       = 0b000000;         // Set all pins LOW

    ANSEL       = 0b10000000;       // AN7 set as input

    OPTION_REG  = 0b00000111;
    TMR0        = 39;
    INTCON      = 0b10100000;

    OSCCON      = 0b01100111;       // 4mhz internal

    uart_init();
    adc_init();

    uart_send_str("Start...\r\n");

    while (1) {
        // Do nothing, it's all taken care of in the interrupt / timer0
    }
    return;
}
