#include <lib/instructions.h>

static char * inst_names[] = { "add", "call", "hcf", "jmp", "lb", "ld", "lw", "mod", "pop", "push", "ret", "sb", "sw", "sub" };

int getinst (char * s) {
    for (int i = 0; i < 15; i++) {
        if (strcmp (s, inst_names [i]) == 0) {
            return i;
        }
    }
    return -1;
}

uint32_t buildinst (uint8_t code, uint8_t op1, uint8_t op2, uint16_t imm) {
printf ("Serializing %u\t%u\t%u\t%u\t\n", code, op1, op2, imm);

    uint32_t i = imm;
    i ^= ((uint32_t)op1 << 16);
    i ^= ((uint32_t)op2 << 22);
    i ^= ((uint32_t)code << 28);

printf ("Out: %u\n", i);

    return i;
}
