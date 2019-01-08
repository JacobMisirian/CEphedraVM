#include <inc/lexer.h>


static void nextid    (lexerstate_t *, token_t *);
static void nextintc  (lexerstate_t *, token_t *);
static void nextstrc  (lexerstate_t *, token_t *);
static void wspace    (lexerstate_t *);
static int readc      (lexerstate_t *);
static int peekc      (lexerstate_t *);
static int peekupc    (lexerstate_t *, int);

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

void lexer_free (lexerstate_t * state) {
    if (state->code != NULL) {
        free (state->code);
    }

    free (state);
}

int lexer_rewind (lexerstate_t * state, int count) {
    if ((state->pos - count) >= 0 && (state->pos - count) < state->len) {
        state->pos -= count;
        return 1;
    }
    return -1;
}

int lexer_forward (lexerstate_t * state, int count) {
    if ((state->pos + count) >= 0 && (state->pos + count) < state->len) {
        state->pos += count;
        return 1;
    }
    return -1;
}

int lexer_gpos (lexerstate_t * state) {
    return state->pos;
}

int lexer_spos (lexerstate_t * state, int pos) {
    if (pos >= 0 && pos < state->len) {
        state->pos = pos;
        return 1;
    }
    return -1;
}

void lexer_nexttok (lexerstate_t * state, token_t * token) {
    wspace (state);

    token->val = 0;

    if (peekc (state) == -1) {
        token->type = eof;
        return;
    }

    char c = (char)peekc (state);

    if (isalpha (c)) {
        nextid (state, token);
    }
    else if (isdigit (c)) {
        nextintc (state, token);
    }
    else if (c == '"') {
        nextstrc (state, token);
    }
    else {
        readc (state);
        switch (c) {
            case '=':
            token->type = assign;
            token->val = (char *)malloc (2);
            token->val [0] = c;
            token->val [1] = 0;
            break;
            case '}':
            token->type = cbrace;
            break;
            case '\'':
            token->type = charc;
            token->val = (char *)malloc (2);
            token->val [0] = (char)readc (state);
            token->val [1] = 0;
            readc (state); // '
            break;
            case ':':
            token->type = colon;
            break;
            case ',':
            token->type = comma;
            break;
            case ')':
            token->type = cparen;
            break;
            case ']':
            token->type = csquare;
            break;
            case '.':
            token->type = dot;
            break;
            case '{':
            token->type = obrace;
            break;
            case '!':
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            if ((char)peekc (state) == '=') {
                token->type = c == '!' ? op : assign;
                token->val = (char *)malloc (3);
                token->val [0] = c;
                token->val [1] = (char)readc(state);
                token->val [2] = 0;
            }
            else {
                token->type = op;
                token->val = (char *)malloc (2);
                token->val [0] = c;
                token->val [1] = 0;
            }
            break;
            case '<':
            case '>':
            token->type = op;
            if ((char) peekc (state) == c) {
                token->val = (char *)malloc (3);
                token->val [0] = c;
                token->val [1] = (char)readc (state);
                token->val [2] = 0;
            }
            else {
                token->val = (char *)malloc (2);
                token->val [0] = c;
                token->val [1] =0;
            }
            break;
            case '(':
            token->type = oparen;
            break;
            case '[':
            token->type = osquare;
            break;
            case ';':
            token->type = semicol;
            break;
            default:
            printf ("Unknown token '%d' in lexer!\n", c);
            token->type = eof;
            break;
        }
    }
}

static void nextid (lexerstate_t * state, token_t * token) {
    int len = 0; // length of identifier.

    // count while letter or digit.
    while (isalnum ((char)readc (state))) {
        len++;
    }

    lexer_rewind (state, len + 1); // rewind to start of identifier.

    // read identifier into heap and null-terminate.
    char * str = (char *)malloc (len + 1);
    int i;
    for (i = 0; i < len; i++) {
        str [i] = (char)readc (state);
    }
    str [i] = 0;

    token->type = id;
    token->val = str;
}

static void nextintc (lexerstate_t * state, token_t * token) {
    int len = 0;

    while (isdigit ((char)readc (state))) {
        len++;
    }

    lexer_rewind (state, len + 1);

    char * str = (char *)malloc (len + 1);
    int i;
    for (i = 0; i < len; i++) {
        str [i] = (char)readc (state);
    }
    str [i] = 0;

    token->type = intc;
    token->val = str;
}

static void nextstrc (lexerstate_t * state, token_t * token) {
    readc (state); // "
    int size = 0;
    // count until " is passed.
    while ((char)readc (state) != '"') {
        size++;
    }

    lexer_rewind (state, size + 1); // rewind to first "

    // read string onto heap and null-terminate.
    char * str = (char *)malloc (size);
    int i;
    for (i = 0; i < size; i++) {
        str [i] = (char)readc (state);
    }
    str [i] = 0;
    readc (state); // "

    token->val = str;
    token->type = stringc;
}

static void wspace (lexerstate_t * state) {
    while (peekc (state) != -1) {
        if (isspace ((char)peekc (state)) == 0) break;
        state->pos++;
    }
}

static int readc (lexerstate_t * state) {
    return state->pos < state->len ? state->code [state->pos++] : -1;
}

static int peekc (lexerstate_t * state) {
    return state->pos < state->len ? state->code [state->pos] : -1;
}

static int peekupc (lexerstate_t * state, int c) {
    return state->pos + c < state->len ? state->code [state->pos + c] : -1;
}
