#ifndef _LEXER_STATE_H_
#define _LEXER_STATE_H_

#include <ctype.h>
#include <lib/registers.h>
#include <lib/instructions.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <lib/token.h>
#include <lib/toktype.h>

typedef struct {
    char * code;
    uint32_t pos;
    uint32_t len;
} lexerstate_t;

lexerstate_t * lexer_init (FILE * f);
void lexer_destruct (lexerstate_t * state);
token_t * lexer_nextstr (lexerstate_t * state);
token_t * lexer_nextident (lexerstate_t * state);

#endif
