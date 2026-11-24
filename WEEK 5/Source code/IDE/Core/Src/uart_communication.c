#include "main.h"
#include "uart_communication.h"
#include "command_parser.h"
#include "timer.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

typedef enum { IDLE, WAIT_OK } CommState;

static CommState state = IDLE;
static char packet[20];
static int packet_len = 0;

static void send_packet(void) {
  HAL_UART_Transmit(&huart2, (uint8_t *)packet, packet_len, 100);
}

static void new_packet(void) {                // read the ADC and build !ADC=xxxx#
  uint32_t ADC_value = HAL_ADC_GetValue(&hadc1);
  packet_len = sprintf(packet, "!ADC=%lu#\r\n", (unsigned long)ADC_value);
}

void uart_communication_fsm(void) {
  /* take the command found by the parser (if any) */
  uint8_t cmd = CMD_NONE;
  if (command_flag) {
    cmd = command_data;
    command_flag = 0;
  }

  switch (state) {
    case IDLE:                                // wait for !RST#
      if (cmd == CMD_RST) {
        new_packet();
        send_packet();
        setTimer(T_TIMEOUT, RESEND_TIMEOUT);
        state = WAIT_OK;
      }
      break;

    case WAIT_OK:                             // wait for !OK#, resend every 3 s
      if (cmd == CMD_OK) {
        state = IDLE;                         // communication finished
      } else if (cmd == CMD_RST) {            // new request: send a new value
        new_packet();
        send_packet();
        setTimer(T_TIMEOUT, RESEND_TIMEOUT);
      } else if (timeUp(T_TIMEOUT)) {         // no !OK# in 3 s: same packet again
        send_packet();
        setTimer(T_TIMEOUT, RESEND_TIMEOUT);
      }
      break;
  }
}
