#ifndef _LEXER_STATE_H_
#define _LEXER_STATE_H_

#include <ctype.h>
#include <inc/registers.h>
#include <inc/instructions.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inc/token.h>
#include <inc/toktype.h>

typedef struct {
    char * code;
    int pos;
    uint32_t len;
} lexerstate_t;

lexerstate_t * lexer_init (FILE * f);
void lexer_destruct (lexerstate_t * state);
int lexer_rewind (lexerstate_t * state, int count);
int lexer_forward (lexerstate_t * state, int count);
int lexer_gpos (lexerstate_t * state);
int lexer_spos (lexerstate_t * state, int pos);
void lexer_nexttok (lexerstate_t * state, token_t * token);

#endif
