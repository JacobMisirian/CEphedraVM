#include <ctype.h>
#include <lib/lexerstate.h>
#include <stdio.h>
#include <stdlib.h>
#include <lib/token.h>
#include <lib/toktype.h>

lexerstate_t * lexer_init (FILE * f);
void lexer_destruct (lexerstate_t * state);
void lexer_wspace (lexerstate_t * state);
int lexer_peekc (lexerstate_t * state);
int lexer_readc (lexerstate_t * state);

lexerstate_t * lexer_init (FILE * f) {
    lexerstate_t * state = (lexerstate_t *)malloc (sizeof (lexerstate_t));
    state->pos = 0;

    fseek (f, 0, SEEK_END);
    state->len = (uint32_t)ftell (f);
    rewind (f);

    int c;
    state->code = (char *)malloc (state->len);
    while ((c = fgetc (f)) != EOF) {
        state->code [state->pos++] = c;
    }

    state->pos = 0;

    return state;
}

void lexer_destruct (lexerstate_t * state) {
    if (state->code != NULL) {
        free (state->code);
    }

    free (state);
}

void lexer_wspace (lexerstate_t * state) {
    while (lexer_peekc (state) != -1) {
        if (isspace ((char)lexer_peekc (state)) == 0) break;
    }
}

int lexer_peekc (lexerstate_t * state) {
    return state->pos < state->len ? state->code[state->pos] : -1;
}

int lexer_readc (lexerstate_t * state) {
    return state->pos < state->len ? state->code[state->pos++] : -1;
}
