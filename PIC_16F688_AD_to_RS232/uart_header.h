#ifndef UART_INIT_HEADER_H
#define UART_INIT_HEADER_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <xc.h>
void uart_init();
void uart_tx(char out);
char uart_rx();
void uart_send_str(const char *);

#ifdef  __cplusplus
}
#endif

#endif  /* UART_INIT_HEADER_H */
