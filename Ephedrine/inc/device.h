#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <inc/cpu.h>

typedef struct {
    void (* power) (void * cpu);
    void (* tick)  (void * cpu);
} device_t;

#endif
