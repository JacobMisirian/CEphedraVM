#include <lib/lexer.h>

static void lexer_wspace (lexerstate_t * state);
static int lexer_peekc (lexerstate_t * state);
static int lexer_readc (lexerstate_t * state);

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

token_t * lexer_nextstr (lexerstate_t * state) {
    lexer_readc (state); // "
    int size = 1; // size of null-terminated string.
    // count until " is passed.
    while ((char)lexer_readc (state) != '"') {
        size++;
    }

    state->pos -= size; // rewind to first "

    // read string onto heap and null-terminate.
    char * str = (char *)malloc (size);
    int i;
    for (i = 0; i < size - 1; i++) {
        str [i] = (char)lexer_readc (state);
    }
    str [i] = 0;
    lexer_readc (state); // "

    token_t * ret = (token_t *)malloc (sizeof (token_t));
    ret->val = str;
    ret->type = string;

    return ret;
}

token_t * lexer_nextident (lexerstate_t * state) {
    lexer_wspace (state);
    int len = 0; // length of identifier.

    // count while letter or digit.
    while (isalnum ((char)lexer_readc (state))) {
        len++;
    }

    state->pos -= (len + 1); // rewind to start of identifier.

    // read identifier into heap and null-terminate.
    char * str = (char *)malloc (len + 1);
    int i;
    for (i = 0; i < len; i++) {
        str [i] = (char)lexer_readc (state);
    }
    str [i] = 0;
    
    token_t * ret = (token_t *)malloc (sizeof (token_t));
    ret->val = str;

    // check token type of identifier.
    if (getregister (str) != -1)
        ret->type = reg;
    else if (getinst (str) != -1)
        ret->type = instruction;
    else
        ret->type = labelreq;

    return ret;
}

static void lexer_wspace (lexerstate_t * state) {
    while (lexer_peekc (state) != -1) {
        if (isspace ((char)lexer_peekc (state)) == 0) break;
        state->pos++;
    }
}

static int lexer_peekc (lexerstate_t * state) {
    return state->pos < state->len ? state->code[state->pos] : -1;
}

static int lexer_readc (lexerstate_t * state) {
    return state->pos < state->len ? state->code[state->pos++] : -1;
}
