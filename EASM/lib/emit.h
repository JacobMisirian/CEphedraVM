#ifndef _EMIT_H_
#define _EMIT_H_

#include <lib/instructions.h>
#include <lib/lexer.h>
#include <lib/registers.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lib/token.h>
#include <lib/toktype.h>

typedef struct {
    uint8_t * bin;
    uint16_t pos;
    uint16_t len;
    lexerstate_t * lexer;
    token_t * tok;
} emitstate_t;

emitstate_t * emit_init (lexerstate_t * lexer);
void emit_destruct (emitstate_t * state);
int emit_len (emitstate_t * state);
uint8_t * emit_assemble (emitstate_t * state);

#endif
