#ifndef INC_UART_COMMUNICATION_H_
#define INC_UART_COMMUNICATION_H_

#define RESEND_TIMEOUT 3000             // ms to wait for !OK#

void uart_communication_fsm(void);      // called in the main loop

#endif
