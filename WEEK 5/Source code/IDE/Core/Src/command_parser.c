#include "main.h"
#include "command_parser.h"
#include <string.h>

extern UART_HandleTypeDef huart2;

/* ---------- reception (interrupt) : Program 1.4 of the manual ---------- */
static uint8_t temp = 0;
static uint8_t buffer[MAX_BUFFER_SIZE];
static volatile uint8_t index_buffer = 0;     // where the interrupt writes next
volatile uint8_t buffer_flag = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART2) {
    buffer[index_buffer++] = temp;
    if (index_buffer == MAX_BUFFER_SIZE) index_buffer = 0;
    buffer_flag = 1;
    HAL_UART_Receive_IT(&huart2, &temp, 1);
  }
}

void command_parser_init(void) {
  HAL_UART_Receive_IT(&huart2, &temp, 1);
}

/* ---------- command parser FSM (main loop) ---------- */
typedef enum { WAIT_START, READ_CMD } ParserState;

static ParserState state = WAIT_START;
static uint8_t index_read = 0;                // where the parser reads next
static char cmd[MAX_CMD_SIZE + 1];
static uint8_t cmd_len = 0;

uint8_t command_flag = 0;
uint8_t command_data = CMD_NONE;

static void parse_char(char c) {
  switch (state) {
    case WAIT_START:                          // wait for '!'
      if (c == '!') {
        cmd_len = 0;
        state = READ_CMD;
      }
      break;

    case READ_CMD:                            // collect until '#'
      if (c == '!') {                         // new start: drop what we had
        cmd_len = 0;
      } else if (c == '#') {                  // end of command
        cmd[cmd_len] = '\0';
        if (strcmp(cmd, "RST") == 0) { command_data = CMD_RST; command_flag = 1; }
        else if (strcmp(cmd, "OK") == 0) { command_data = CMD_OK; command_flag = 1; }
        state = WAIT_START;                   // unknown command: ignored
      } else if (cmd_len < MAX_CMD_SIZE) {
        cmd[cmd_len++] = c;
      } else {                                // too long: not a valid command
        state = WAIT_START;
      }
      break;
  }
}

void command_parser_fsm(void) {
  while (index_read != index_buffer) {        // every character not read yet
    parse_char((char)buffer[index_read++]);
    if (index_read == MAX_BUFFER_SIZE) index_read = 0;
  }
}
