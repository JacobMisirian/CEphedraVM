#include <lib/cpu.h>

static void parseinst (uint32_t inst, uint8_t * code, uint8_t * op1, uint8_t * op2, uint16_t * imm);

cpustate_t * cpu_init (char os[], size_t ossize, size_t rsize) {
    cpustate_t * state = (cpustate_t *)malloc (sizeof (cpustate_t));
    state->ram = (uint8_t *)malloc (rsize);
    memcpy (state->ram, os, ossize);
    return state;
}

void cpu_free (cpustate_t * state) {
    free (state->ram);
    free (state);
}

void cpu_power (cpustate_t * state) {
    uint32_t inst;
    uint16_t imm;
    uint8_t code, op1, op2;
    uint8_t buffer [2];

    state->registers[R_IP] = 0;
    while (1) {
        inst = *(uint32_t*)(state->ram + state->registers[R_IP]);
        parseinst (inst, &code, &op1, &op2, &imm);
        printf ("Int: %u, Opcode %u, Op1: %u, Op2: %u, Imm: %u\n", inst, code, op1, op2, imm);

	uint16_t first, second;

        if ((code >> 7) & 1) {
            first = state->registers [op1];
            second = imm;
            code ^= (1 << 7);
        }
        else if ((code >> 6) & 1) {
            first = imm;
            second = state->registers [op2];
        } else {
            first = state->registers [op1];
            second = state->registers [op2];
        }

        switch (code) {
            case INST_ADD:
                state->registers [op1] += second;
                break;
            case INST_CALL:

                break;
            case INST_HCF:
                for (int i = 0; i < 0x10; i++) {
                    printf ("Register %d: %d\n", i, state->registers [i]);
                }
                return;
            case INST_JMP:
                state->registers[R_IP] = imm;
                continue;
            case INST_LB:
                state->ram [state->registers [op1]] = second;
                break;
                break;
            case INST_LI:
                state->registers [op1] = imm;
                break;
            case INST_LW:
                memcpy ((unsigned char *)buffer, (unsigned char *)&(second), 2);
                state->ram [state->registers [op1]]     = buffer [0];
                state->ram [state->registers [op1] + 1] = buffer [1];
                break;
            case INST_MOD:
                state->registers [op1] %= second;
                break;
            case INST_MOV:
                state->registers [op1] = state->registers [op2];
                break;
            case INST_POP:
                state->registers [op1] = ((uint16_t)state->ram [state->registers[R_STACK]++] << 8) ^ 
                                                    (uint16_t)state->ram [state->registers[R_STACK]++];
                break;
            case INST_PUSH:
                state->registers[R_STACK] -= 2;
                memcpy ((unsigned char *)buffer, (unsigned char *)&(first), 2);
                state->ram [state->registers[R_STACK]]     = buffer [0];
                state->ram [state->registers[R_STACK] + 1] = buffer [1];
                break;
            case INST_RET:
                state->registers[R_IP] = ((uint16_t)state->ram [state->registers[R_STACK]++] << 8) ^ (uint16_t)state->ram [state->registers[R_STACK]++];
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
    }
}

static void parseinst (uint32_t inst, uint8_t * code, uint8_t * op1, uint8_t * op2, uint16_t * imm) {
    (*code) = (uint8_t)(inst >> 28);
    (*op1) = (uint8_t)((inst >> 16) & 63);
    (*op2) = (uint8_t)((inst >> 22) & 63);
    (*imm) = (uint16_t)(inst & 0xFF);
}
