#ifndef _CPU_H_
#define _CPU_H_

#include <lib/instructions.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INST_SIZE 0x4

#define R_IP 0xE
#define R_STACK 0XF

typedef struct {
    uint8_t * ram;
    int16_t registers [0x10];
} cpustate_t;

cpustate_t * cpu_init (char os[], size_t ossize, size_t rsize);
void cpu_free (cpustate_t * state);
void cpu_power (cpustate_t * state);

#endif
