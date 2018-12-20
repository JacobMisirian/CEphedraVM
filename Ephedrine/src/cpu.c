#include <lib/cpu.h>

static void parseinst (uint32_t inst, uint8_t * code, uint8_t * op1, uint8_t * op2, uint16_t * imm);

cpustate_t * cpu_init (char os[], size_t ossize, size_t rsize) {
    cpustate_t * state = (cpustate_t *)malloc (sizeof (cpustate_t));

    state->ram = (uint8_t *)malloc (rsize);
    memcpy (state->ram, os, ossize);
    state->screen = textscreen_init ();

    return state;
}

void cpu_free (cpustate_t * state) {
    free (state);
}

void cpu_power (cpustate_t * state) {
    state->screen->power (state);

    uint32_t inst;
    uint16_t imm;
    uint8_t code, op1, op2;
    uint8_t buffer [2];

    state->registers[R_IP] = 0;
    while (1) {
        inst = *(uint32_t*)(state->ram + state->registers[R_IP]);
        parseinst (inst, &code, &op1, &op2, &imm);

	uint16_t first, second;
        if ((op1 >> 5) & 1) {
            op1 ^= (1 << 5);
            first = imm;
            second = state->registers [op2];
        }
        else if ((op2 >> 5) & 1) {
            op2 ^= (1 << 5);
            first = state->registers [op1];
            second = imm;
        } else {
            first = state->registers [op1];
            second = state->registers [op2];
        }

        printf ("Int: %u, Opcode %u, Op1: %u, Op2: %u, Imm: %u\n", inst, code, op1, op2, imm);

        switch (code) {
            case INST_ADD:
                state->registers [op1] += second;
                break;
            case INST_CALL:
                state->registers [R_STACK] -= 2;
                state->registers [R_IP] += INST_SIZE;
                memcpy ((unsigned char *)buffer, (unsigned char *)&(state->registers [R_IP]), 2);
                state->ram [state->registers [R_STACK]] = buffer [0];
                state->ram [state->registers [R_STACK] + 1] = buffer [1];
                state->registers [R_IP] = imm;
                continue;
            case INST_HCF:
                for (int i = 0; i < 0x10; i++) {
                    printf ("Register %d: %d\n", i, state->registers [i]);
                }
                return;
            case INST_JMP:
                state->registers[R_IP] = imm;
                continue;
            case INST_LB:
                state->registers [op1] = state->ram [second];
                break;;
            case INST_LD:
                state->registers [op1] = second;
                break;
            case INST_LW:
                state->registers [op1] = (uint16_t)state->ram [second] ^ (((uint16_t)state->ram [second + 1]) << 8);
                break;
            case INST_MOD:
                state->registers [op1] %= second;
                break;
            case INST_POP:
                state->registers [op1] = (uint16_t)state->ram [state->registers [R_STACK]++] ^ 
                                                    (((uint16_t)state->ram [state->registers [R_STACK]++]) << 8);
                break;
            case INST_PUSH:
                state->registers[R_STACK] -= 2;
                memcpy ((unsigned char *)buffer, (unsigned char *)&(first), 2);
                state->ram [state->registers[R_STACK]]     = buffer [0];
                state->ram [state->registers[R_STACK] + 1] = buffer [1];
                break;
            case INST_RET:
                state->registers[R_IP] = (uint16_t)state->ram [state->registers [R_STACK]++] ^ (((uint16_t)state->ram [state->registers [R_STACK]++]) << 8);
                continue;
            case INST_SB:
                state->ram [state->registers [op1]] = second;
                break;
            case INST_SW:
                memcpy ((unsigned char *)buffer, (unsigned char *)&(second), 2);
                state->ram [state->registers [op1]]     = buffer [0];
                state->ram [state->registers [op1] + 1] = buffer [1];
                break;
            case INST_SUB:
                state->registers [op1] -= second;
                break;
        }

        state->registers[R_IP] += INST_SIZE;
        state->screen->tick (state);
   }
}

static void parseinst (uint32_t inst, uint8_t * code, uint8_t * op1, uint8_t * op2, uint16_t * imm) {
    (*code) = (uint8_t)(inst >> 28);
    (*op1) = (uint8_t)((inst >> 16) & 63);
    (*op2) = (uint8_t)((inst >> 22) & 63);
    (*imm) = (uint16_t)(inst & 0xFFFF);
}
