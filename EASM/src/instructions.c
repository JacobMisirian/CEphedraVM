#include <lib/instructions.h>

static char * inst_names[] = { "add", "addi", "hcf", "jmp", "lb", "lbi", "li", "lw", "lwi", "mod", "modi", "mov", "pop", "push", "pushi", "sb", "sbi", "sw", "swi", "sub", "subi" };

int8_t getinst (char * s) {
    for (int8_t i = 0; i < 21; i++) {
        if (strcmp (s, inst_names [i]) == 0) {
            return i;
        }
    }
    return -1;
}
