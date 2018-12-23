#include <inc/textscreen.h>

static uint8_t buffer [TEXT_WIDTH * TEXT_HEIGHT];


void textscreen_power (void * cpustate) {
    
}

void textscreen_tick (void * cpustate) {
    for (int row = 0; row < TEXT_HEIGHT; row++) {
        for (int col = 0; col < TEXT_WIDTH; col++) {
            uint8_t buffc = buffer [(row * TEXT_WIDTH) + col];
            uint8_t realc = ((cpustate_t *)cpustate)->ram[MMIO_ADDR + ((row * TEXT_WIDTH) + col)];

            if (buffc != realc) {
                buffer [(row * TEXT_WIDTH) + col] = realc;
                printf("\033[%d;%dH%c", row, col, realc);
                fflush (stdout);
            }
        }
    }
}

device_t * textscreen_init () {
    device_t * screen = (device_t *)malloc (sizeof (device_t));
    
    screen->power = textscreen_power;
    screen->tick = textscreen_tick;

    return screen;
}
