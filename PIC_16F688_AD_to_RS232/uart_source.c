#include "uart_header.h"

// Function to initialize the uart port
void uart_init() {
    TXSTAbits.BRGH  = 0; // high baud selection bit, 1=high, 0=low
    TXSTAbits.SYNC  = 0; // USART mode selection bit, 1=sync mode, 0=async mode
    TXSTAbits.TX9   = 0; // 9-bit selection bit, 1=9-bit transmission, 0=8-bit trans
    RCSTAbits.CREN  = 1; // continous receive enable bit, 1=Enable continous receive.
    /* 4Mhz -> 9600
     * 4000000 / 16 = 250000
     * 250000 / 9600 = 26.0416
     * 26.0416 - 1 = 25
     *
     * 8MHz -> 9600
     * 8000000 / 64 = 125000
     * 125000 / 9600 = 13.0208
     * 13.0208 - 1 = 12
     */

    SPBRG = 12; // 9600-n-8-1
    PIE1bits.RCIE   = 1; // USART Receive iterrupt enable bit, 1=enable, 0=disable
    RCSTAbits.SPEN  = 1; // serial port enable bit, 1=serial port enable, 0=disable
    TXSTAbits.TXEN  = 1; // transmit enable bit, 1=Transmit enable, 0=disable
    return;
}

// Function to send one char
void uart_tx(char out) {
    while (TXIF == 0);
    TXREG = out;
}

// Function to recieve one char
char uart_rx() {
    while (RCIF == 0);
    if (RCSTAbits.OERR) {
        CREN = 0;
        NOP();
        CREN = 1;
    }
    return (RCREG);
}

// Function to send a string of single chars
void uart_send_str(const char *out) {
    while (*out!='\0') {
        uart_tx(*out);
        out++;
    }
}
