#include <inc/lexer.h>

static void lexer_nextstr      (lexerstate_t *, token_t *);
static void lexer_nextident    (lexerstate_t *, token_t *);
static void lexer_nextlabeldec (lexerstate_t *, token_t *);
static void lexer_nextinteger  (lexerstate_t *, token_t *);
static void lexer_wspace  (lexerstate_t *);
static void lexer_comment (lexerstate_t *);
static int lexer_peekc    (lexerstate_t *);
static int lexer_readc    (lexerstate_t *);

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
    lexer_wspace (state); // burn whitespace.

    if (lexer_peekc (state) == -1) {
        token->type = eof;
        token->val = 0;
        return;
    }

    char c = (char)lexer_peekc (state);

    if (c == ',') {
        token->type = comma;
        token->val = 0;
        lexer_readc (state);
    }
    else if (c == '"') {
        lexer_nextstr (state, token);
    }
    else if (isalpha (c)) {
        lexer_nextident (state, token);
    }
    else if (c == '.') {
        lexer_nextlabeldec (state, token);
    }
    else if (isdigit (c)) {
        lexer_nextinteger (state, token);
    }
    else if (c == ';') {
        lexer_comment (state);
        lexer_nexttok (state, token);
    }
    else {
        token->type = error;
        token->val = 0;
    }
}

static void lexer_nextstr (lexerstate_t * state, token_t * token) {
    lexer_readc (state); // "
    int size = 1; // size of null-terminated string.
    // count until " is passed.
    while ((char)lexer_readc (state) != '"') {
        size++;
    }

    lexer_rewind (state, size); // rewind to first "

    // read string onto heap and null-terminate.
    char * str = (char *)malloc (size);
    int i;
    for (i = 0; i < size - 1; i++) {
        str [i] = (char)lexer_readc (state);
    }
    str [i] = 0;
    lexer_readc (state); // "

    token->val = str;
    token->type = string;
}

static void lexer_nextident (lexerstate_t * state, token_t * token) {
    lexer_wspace (state);
    int len = 0; // length of identifier.

    // count while letter or digit.
    while (isalnum ((char)lexer_readc (state))) {
        len++;
    }

    lexer_rewind (state, len + 1); // rewind to start of identifier.

    // read identifier into heap and null-terminate.
    char * str = (char *)malloc (len + 2); // 1 extra byte so emit can append 'i'.
    int i;
    for (i = 0; i < len; i++) {
        str [i] = (char)lexer_readc (state);
    }
    str [i] = 0;
    
    token->val = str;

    // check token type of identifier.
    if (getregister (str) != -1)
        token->type = reg;
    else if (getinst (str) != -1)
        token->type = instruction;
    else
        token->type = labelreq;
}

static void lexer_nextlabeldec (lexerstate_t * state, token_t * token) {
    lexer_readc (state); // .

    lexer_nextident (state, token); // label name will read as toktype::labelreq.
    token->type = labeldec; // change identifier token from labelreq to labeldec.
}

static void lexer_nextinteger (lexerstate_t * state, token_t * token) {
    int len = 0; // length of integer.
    
    // count while digit.
    while (isdigit ((char)lexer_readc (state))) {
        len++;
    }

    lexer_rewind (state, len + 1); // isdigit returned false, meaning lexer_readc burned another char.
    
    // read integer string into heap.
    char * str = (char *)malloc (len + 1);
    int i;
    for (i = 0; i < len; i++) {
        str [i] = (char)lexer_readc (state);
    }
    str [i] = 0;

    token->val = str;
    token->type = integer;
}

static void lexer_wspace (lexerstate_t * state) {
    while (lexer_peekc (state) != -1) { 
        if (isspace ((char)lexer_peekc (state)) == 0) break;
        state->pos++;
    }
}

static void lexer_comment (lexerstate_t * state) {
    lexer_readc (state); // ;
    while (lexer_peekc (state) != -1) {
        if ((char)lexer_readc (state) == '\n') break;
    }
}

static int lexer_peekc (lexerstate_t * state) {
    return state->pos < state->len ? state->code[state->pos] : -1;
}

static int lexer_readc (lexerstate_t * state) {
    return state->pos < state->len ? state->code[state->pos++] : -1;
}
