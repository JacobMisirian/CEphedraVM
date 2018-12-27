#ifndef _CPU_H_
#define _CPU_H_

#include <inc/device.h>
#include <inc/instructions.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inc/textscreen.h>

#define INST_SIZE 0x4

#define R_FLAGS 0xC
#define R_BP 0xD
#define R_IP 0xE
#define R_STACK 0XF

#define F_ZERO 0
#define F_SIGN 1

typedef struct {
    uint8_t * ram;
    int16_t registers [0x10];
    device_t * screen;
} cpustate_t;

cpustate_t * cpu_init (char os[], size_t ossize, size_t rsize);
void cpu_free (cpustate_t * state);
void cpu_power (cpustate_t * state);

#endif
