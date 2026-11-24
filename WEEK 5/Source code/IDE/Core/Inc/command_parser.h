#ifndef INC_COMMAND_PARSER_H_
#define INC_COMMAND_PARSER_H_

#include <stdint.h>

#define MAX_BUFFER_SIZE  30
#define MAX_CMD_SIZE     10

/* commands found by the parser */
enum { CMD_NONE = 0, CMD_RST, CMD_OK };

extern volatile uint8_t buffer_flag;    // 1 = new character(s) received
extern uint8_t command_flag;            // 1 = a new command is ready
extern uint8_t command_data;            // CMD_RST or CMD_OK

void command_parser_init(void);         // starts UART reception
void command_parser_fsm(void);          // called in the main loop

#endif
