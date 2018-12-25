#include <inc/parser.h>

static int match (parserstate_t * state, toktype_t type);
static int matchv (parserstate_t * state, toktype_t type, const char * val);
static int accept (parserstate_t * state, toktype_t type);
static int acceptv (parserstate_t * state, toktype_t type, const char * val);

parserstate_t * parser_init (lexerstate_t * lexer) {
    blockstate_t * blockstate = (blockstate_t *)malloc (sizeof (blockstate_t));

    astnode_t * ast = (astnode_t *)malloc (sizeof (astnode_t));
    ast->state = blockstate;
    ast->type = blocknode;

    parserstate_t * parser = (parserstate_t *)malloc (sizeof (parserstate_t));
    parser->lexer = lexer;
    parser->ast = ast;
    parser->tok = (token_t *)malloc (sizeof (token_t));
    lexer_nexttok (lexer, parser->tok);

    return parser;
}

void parser_free (parserstate_t * state) {
    free (state);
}

void parser_parse (parserstate_t * state) {

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
