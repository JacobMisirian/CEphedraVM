#include <inc/parser.h>

static astnode_t * parsefuncdec (parserstate_t *);
static astnode_t * parsestmt (parserstate_t *);
static astnode_t * parseblock (parserstate_t *);
static astnode_t * parsecond (parserstate_t *);
static astnode_t * parsefloop (parserstate_t *);
static astnode_t * parseret (parserstate_t *);
static astnode_t * parsewloop (parserstate_t *);
static astnode_t * parseexp (parserstate_t *);
static astnode_t * parseassign (parserstate_t *);
static astnode_t * parseeq (parserstate_t *);
static astnode_t * parsecomp (parserstate_t *);
static astnode_t * parseor (parserstate_t *);
static astnode_t * parsexor (parserstate_t *);
static astnode_t * parseand (parserstate_t *);
static astnode_t * parseshift (parserstate_t *);
static astnode_t * parseadd (parserstate_t *);
static astnode_t * parsemul (parserstate_t *);
static astnode_t * parseuop (parserstate_t *);
static astnode_t * parseaccess (parserstate_t *, astnode_t *);
static astnode_t * parseterm (parserstate_t *);
static llist parseargs (parserstate_t *);
static int match (parserstate_t *, toktype_t);
static int matchv (parserstate_t *, toktype_t, const char *);
static int accept (parserstate_t *, toktype_t);
static int acceptv (parserstate_t *, toktype_t, const char *);
static const char * expect (parserstate_t *, toktype_t);
static const char * expectv (parserstate_t *, toktype_t, const char *);


static void pause (const char * s) {
    printf ("%s\n", s);
    int i;
    scanf ("%d", &i);
}

parserstate_t * parser_init (lexerstate_t * lexer) {
    parserstate_t * parser = (parserstate_t *)malloc (sizeof (parserstate_t));
    parser->children = NULL;
    parser->lexer = lexer;
    parser->tok = (token_t *)calloc (1, sizeof (token_t));

    return parser;
}

void parser_free (parserstate_t * state) {
    free (state);
}

void parser_parse (parserstate_t * state) {
    do {
        lexer_nexttok (state->lexer, state->tok);
        
        state->children = llist_add (state->children, parsefuncdec (state));

    } while (!match (state, eof));
}

static astnode_t * parsefuncdec (parserstate_t * state) {
    char * name = state->tok->val;
    expect (state, id);

    llist args = parseargs (state);
    astnode_t * body = parsestmt (state);

    return funcdecnode_init (name, args, body);
}

static astnode_t * parsestmt (parserstate_t * state) {
    if (matchv (state, id, "if")) {
        return parsecond (state);
    }
    else if (matchv (state, id, "for")) {
        return parsefloop (state);
    }
    else if (matchv (state, id, "return")) {
        return parseret (state);
    }
    else if (matchv (state, id, "while")) {
        return parsewloop (state);
    }
    else if (match (state, obrace)) {
        return parseblock (state);
    }
    else {
        return parseexp (state);
    }
}

static astnode_t * parseblock (parserstate_t * state) {
    expect (state, obrace);

    llist l = NULL;
    while (!accept (state, cbrace)) {
        l = llist_add (l, parsestmt (state));
    }

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

static astnode_t * parseret (parserstate_t * state) {
    expectv (state, id, "return");
    
    astnode_t * val = parseexp (state);
    
    return retnode_init (val);
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
    return parseassign (state);
}

static astnode_t * parseassign (parserstate_t * state) {
    astnode_t * left = parseeq (state);

    if (accept (state, assign)) {
        return assignnode_init (left, parseassign (state));
    }

    return left;
}

static astnode_t * parseeq (parserstate_t * state) {
    astnode_t * left = parsecomp (state);

    while (match (state, op)) {
        if (acceptv (state, op, "==")) {
            return binopnode_init (eq, left, parseeq (state));
        }
        else if (acceptv (state, op, "!=")) {
            return binopnode_init (neq, left, parseeq (state));
        }
        else {
            break;
        }
    }

    return left;
}

static astnode_t * parsecomp (parserstate_t * state) {
    astnode_t * left = parseor (state);

    while (match (state, op)) {
        if (acceptv (state, op, "<")) {
            return binopnode_init (less, left, parsecomp (state));
        }
        else if (acceptv (state, op, "<=")) {
            return binopnode_init (lesseq, left, parsecomp (state));
        }
        else if (acceptv (state, op, ">")) {
            return binopnode_init (great, left, parsecomp (state));
        }
        else if (acceptv (state, op, ">=")) {
            return binopnode_init (greateq, left, parsecomp (state));
        }
        else {
            break;
        }
    }

    return left;
}

static astnode_t * parseor (parserstate_t * state) {
    astnode_t * left = parsexor (state);

    while (acceptv (state, op, "|")) {
        left = binopnode_init (or, left, parseor (state));
    }

    return left;
}

static astnode_t * parsexor (parserstate_t * state) {
    astnode_t * left = parseand (state);

    while (acceptv (state, op, "^")) {
        left = binopnode_init (xor, left, parsexor (state));
    }

    return left;
}

static astnode_t * parseand (parserstate_t * state) {
    astnode_t * left = parseshift (state);
    
    while (acceptv (state, op, "&")) {
        left = binopnode_init (and, left, parseand (state));
    }

    return left;
}

static astnode_t * parseshift (parserstate_t * state) {
    astnode_t * left = parseadd (state);

    while (match (state, op)) {
        if (acceptv (state, op, "<<")) {
            return binopnode_init (shil, left, parseshift (state));
        }
        else if (acceptv (state, op, ">>")) {
            return binopnode_init (shir, left, parseshift (state));
        }
        else {
            break;
        }
    }
    
    return left;
}

static astnode_t * parseadd (parserstate_t * state) {
    astnode_t * left = parsemul (state);

    while (match (state, op)) {
        if (acceptv (state, op, "+")) {
            return binopnode_init (add, left, parseadd (state));
        }
        else if (acceptv (state, op, "-")) {
            return binopnode_init (sub, left, parseadd (state));
        }
        else {
            break;
        }
    }

    return left;
}

static astnode_t * parsemul (parserstate_t * state) {
    astnode_t * left = parseuop (state);

    while (match (state, op)) {
        if (acceptv (state, op, "*")) {
            return binopnode_init (mul, left, parsemul (state));
        }
        else if (acceptv (state, op, "/")) {
            return binopnode_init (divide, left, parsemul (state));
        }
        else {
            break;
        }
    }

    return left;
}

static astnode_t * parseuop (parserstate_t * state) {
    if (acceptv (state, op, "--")) {
        return uopnode_init (dec, parseuop (state));
    }
    else if (acceptv (state, op, "++")) {
        return uopnode_init (inc, parseuop (state));
    }
    else {
        return parseaccess (state, NULL);
    }
}

static astnode_t * parseaccess (parserstate_t * state, astnode_t * left) {
    if (left == NULL) left = parseterm (state);

    if (match (state, oparen)) {
        return parseaccess (state, funccallnode_init (left, parseargs (state)));
    }
    else if (accept (state, osquare)) {
        astnode_t * val = parseexp (state);
        expect (state, csquare);
        return parseaccess (state, subscrnode_init (left, val));
    }
    else {
        return left;
    }
}

static astnode_t * parseterm (parserstate_t * state) {
    if (match (state, charc)) {
        return charcnode_init (expect (state, charc) [0]);
    }
    else if (match (state, intc)) {
        return intcnode_init (atoi (expect (state, intc)));
    }
    else if (match (state, stringc)) {
        return stringcnode_init (expect (state, stringc));
    }
    else if (match (state, id)) {
        return idnode_init (expect (state, id));
    }
    else {
        printf ("Parser error! Unexpected token %d with val '%s'!\n", state->tok->type, state->tok->val);
        lexer_nexttok (state->lexer, state->tok);
        return NULL;       
    }
}

static llist parseargs (parserstate_t * state) {
    expect (state, oparen);

    llist l = NULL;
    while (!accept (state, cparen)) {
        l = llist_add (l, parseexp (state));
    }

    return l;
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

static const char * expect (parserstate_t * state, toktype_t type) {
    if (!match (state, type)) {
        printf ("Parser error! Expected %d, got %d with val '%s'!\n", type, state->tok->type, state->tok->val);
        return NULL;
    }

    const char * ret = state->tok->val;
    lexer_nexttok (state->lexer, state->tok);
    return ret;
}

static const char * expectv (parserstate_t * state, toktype_t type, const char * val) {
    if (!matchv (state, type, val)) {
        printf ("Parser error! Expected %d with val '%s', got %d with val '%s'!\n", type, val, state->tok->type, state->tok->val);
        return NULL;
    }
 
    const char * ret = state->tok->val;
    lexer_nexttok (state->lexer, state->tok);
    return ret;
}
