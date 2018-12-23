#ifndef _TEXTSCREEN_H_
#define _TEXTSCREEN_H_

#include <inc/cpu.h>
#include <inc/device.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MMIO_ADDR 4000
#define TEXT_WIDTH 80
#define TEXT_HEIGHT 25

device_t * textscreen_init ();
void textscreen_power (void * cpustate);
void textscreen_tick (void * cpustate);

#endif
