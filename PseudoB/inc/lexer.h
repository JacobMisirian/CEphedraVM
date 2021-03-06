#ifndef _LEXER_H_
#define _LEXER_H_

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inc/token.h>
#include <inc/toktype.h>

typedef struct {
    char * code;
    int pos;
    int len;
} lexerstate_t;

lexerstate_t * lexer_init (FILE * f);
void lexer_free (lexerstate_t * state);
int lexer_rewind (lexerstate_t * state, int count);
int lexer_forward (lexerstate_t * state, int count);
int lexer_gpos (lexerstate_t * state);
int lexer_spos (lexerstate_t * state, int pos);
void lexer_nexttok (lexerstate_t * state, token_t * token);

#endif
