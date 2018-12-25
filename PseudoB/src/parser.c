#include <inc/parser.h>

static astnode_t * parsestmt (parserstate_t *);
static astnode_t * parseblock (parserstate_t *);
static astnode_t * parsecond (parserstate_t *);
static astnode_t * parsefloop (parserstate_t *);
static astnode_t * parsewloop (parserstate_t *);
static astnode_t * parseexp (parserstate_t *);
static int match (parserstate_t *, toktype_t);
static int matchv (parserstate_t *, toktype_t, const char *);
static int accept (parserstate_t *, toktype_t);
static int acceptv (parserstate_t *, toktype_t, const char *);
static int expect (parserstate_t *, toktype_t);
static int expectv (parserstate_t *, toktype_t, const char *);


parserstate_t * parser_init (lexerstate_t * lexer) {
    parserstate_t * parser = (parserstate_t *)malloc (sizeof (parserstate_t));
    parser->children = NULL;
    parser->lexer = lexer;
    parser->tok = (token_t *)malloc (sizeof (token_t));

    return parser;
}

void parser_free (parserstate_t * state) {
    free (state);
}

void parser_parse (parserstate_t * state) {
    do {
        lexer_nexttok (state->lexer, state->tok);
        
        state->children = llist_add (state->children, parsestmt (state));

    } while (!match (state, eof));
}

static astnode_t * parsestmt (parserstate_t * state) {
    if (matchv (state, id, "if")) {
        return parsecond (state);
    }
    else if (matchv (state, id, "for")) {
        return parsefloop (state);
    }
    else if (matchv (state, id, "while")) {
        return parsewloop (state);
    }
    else if (match (state, obrace)) {
        return parseblock (state);
    }
    else {
        printf ("Parser error! Unexpected token %d with val '%s'!\n", state->tok->type, state->tok->val);
        return NULL;
    }
}

static astnode_t * parseblock (parserstate_t * state) {
    expect (state, obrace);

    llist l = NULL;
    do {
        l = llist_add (l, parsestmt (state));
    } while (!match (state, obrace));

    expect (state, cbrace);

    return blocknode_init (l);
}

static astnode_t * parsecond (parserstate_t * state) {
    expectv (state, id, "if");

    expect (state, oparen);
    astnode_t * cond = parseexp (state);
    expect (state, cparen);

    astnode_t * body = parsestmt (state);
    astnode_t * elsebody = NULL;
    if (acceptv (state, id, "else")) {
        elsebody = parsestmt (state);
    }

    return condnode_init (cond, body, elsebody);
}

static astnode_t * parsefloop (parserstate_t * state) {
    expectv (state, id, "for");

    expect (state, oparen);
    astnode_t * prestmt = parsestmt (state);
    astnode_t * cond = parseexp (state);
    astnode_t * repstmt = parsestmt (state);
    expect (state, cparen);

    astnode_t * body = parsestmt (state);

    return floopnode_init (prestmt, cond, repstmt, body);
}

static astnode_t * parsewloop (parserstate_t * state) {
    expectv (state, id, "while");

    expect (state, oparen);
    astnode_t * cond = parseexp (state);
    expect (state, cparen);

    astnode_t * body = parsestmt (state);

    return wloopnode_init (cond, body);
}

static astnode_t * parseexp (parserstate_t * state) {

}

static int match (parserstate_t * state, toktype_t type) {
    return state->tok->type == type;
}

static int matchv (parserstate_t * state, toktype_t type, const char * val) {
    return (state->tok->type == type) && (strcmp (state->tok->val, val) == 0);
}

static int accept (parserstate_t * state, toktype_t type) {
    if (match (state, type)) {
        lexer_nexttok (state->lexer, state->tok);
        return 1;
    }
    return 0;
}

static int acceptv (parserstate_t * state, toktype_t type, const char * val) {
    if (matchv (state, type, val)) {
        lexer_nexttok (state->lexer, state->tok);
        return 1;
    }
    return 0;
}

static int expect (parserstate_t * state, toktype_t type) {
    if (!match (state, type)) {
        printf ("Parser error! Expected %d, got %d with val '%s'!\n", type, state->tok->type, state->tok->val);
        return -1;
    }

    lexer_nexttok (state->lexer, state->tok);
    return 1;
}

static int expectv (parserstate_t * state, toktype_t type, const char * val) {
    if (!matchv (state, type, val)) {
        printf ("Parser error! Expected %d with val '%s', got %d with val '%s'!\n", type, val, state->tok->type, state->tok->val);
        return -1;
    }
    
    lexer_nexttok (state->lexer, state->tok);
    return 1;
}
