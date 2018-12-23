#include <inc/registers.h>

static char * register_names[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "flags", "bp", "ip", "sp" };

int getregister (char * s) {
    for (int i = 0; i < 0x10; i++) {
        if (strcmp (s, register_names [i]) == 0) {
            return i;
        }
    }
    return -1;
}
