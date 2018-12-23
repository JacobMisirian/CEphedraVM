#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include <stdint.h>
#include <string.h>

#include <stdio.h>

#define INST_SIZE 0x04;

int getinst (char * s);
uint32_t buildinst (uint8_t code, uint8_t op1, uint8_t op2, uint16_t imm);

#endif
