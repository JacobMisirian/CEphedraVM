#include <lib/instructions.h>

static char * inst_names[] = { "add", "addi", "hcf", "jmp", "lb", "lbi", "li", "lw", "lwi", "mod", "modi", "mov", "pop", "push", "pushi", "sb", "sbi", "sw", "swi", "sub", "subi" };

int getinst (char * s) {
    for (int i = 0; i < 21; i++) {
        if (strcmp (s, inst_names [i]) == 0) {
            return i;
        }
    }
    return -1;
}

uint32_t buildinst (uint8_t code, uint8_t op1, uint8_t op2, uint16_t imm) {
printf ("Serializing %u\t%u\t%u\t%u\t\n", code, op1, op2, imm);

    uint32_t i = imm;
    i ^= ((uint32_t)op1 << 0x10);
    i ^= ((uint32_t)op2 << 0x16);
    i ^= ((uint32_t)code << 0x1C);

printf ("Out: %u\n", i);

    return i;
}
