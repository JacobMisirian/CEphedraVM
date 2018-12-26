#include <inc/emit.h>

static void handle (emitstate_t *, astnode_t *);
static void hassign (emitstate_t *, astnode_t *);
static void hbinop (emitstate_t *, astnode_t *);
static void hblock (emitstate_t *, astnode_t *);
static void hcharc (emitstate_t *, astnode_t *);
static void hcond (emitstate_t *, astnode_t *);
static void hfloop (emitstate_t *, astnode_t *);
static void hfunccall (emitstate_t *, astnode_t *);
static void hfuncdec (emitstate_t *, astnode_t *);
static void hid (emitstate_t *, astnode_t *);
static void hintc (emitstate_t *, astnode_t *);
static void hret (emitstate_t *, astnode_t *);
static void hstringc (emitstate_t *, astnode_t *);
static void hsubscr (emitstate_t *, astnode_t *);
static void huop (emitstate_t *, astnode_t *);
static void hwloop (emitstate_t *, astnode_t *);
static int pushreg (emitstate_t *);
static int popreg (emitstate_t *);
static char * gensym (emitstate_t *);

emitstate_t * emit_init (parserstate_t * parser) {
    emitstate_t * state = (emitstate_t *)malloc (sizeof (emitstate_t));
    state->ast = parser->children;
    state->curreg = 0;
    state->lblkeys = NULL;
    state->lblvals = NULL;

    return state;
}

void emit_free (emitstate_t * state) {
    free (state->lblkeys);
    free (state->lblvals);
    free (state);
}

void emit_run (emitstate_t * state) {
    size_t clen = llist_size (state->ast);
    for (int i = 0; i < clen; i++) {
        astnode_t * node = (astnode_t *)llist_get (state->ast, i);
        handle (state, node);
    }

    size_t slen = llist_size (state->lblkeys);
 
    for (int i = 0; i < slen; i++) {
        printf (".%s\n", (char *)llist_get (state->lblkeys, i));
        printf ("\"%s\"\n", (char *)llist_get (state->lblvals, i));
    }
}

static void hassign (emitstate_t * state, astnode_t * node) {}

static void hbinop (emitstate_t * state, astnode_t * node) {
    binopstate_t * binopstate = (binopstate_t *)node->state;

    int r0 = pushreg (state);
    int r1 = popreg (state);

    handle (state, binopstate->right);
    handle (state, binopstate->left);

    printf ("pop r%d\n", r0);
    printf ("pop r%d\n", r1);

    switch (binopstate->type) {
    case less:
        printf ("");
        break;
    }
}

static void hblock (emitstate_t * state, astnode_t * node) {
    blockstate_t * blockstate = (blockstate_t *)node->state;

    size_t len = llist_size (blockstate->members);
    for (int i = 0; i < len; i++) {
        handle (state, llist_get (blockstate->members, i));
    }
}

static void hcharc (emitstate_t * state, astnode_t * node) {}

static void hcond (emitstate_t * state, astnode_t * node) {
    condstate_t * condstate = (condstate_t *)node->state;

    char * elselbl = gensym (state);
    char * endlbl = gensym (state);
    int r = pushreg (state);

    handle (state, condstate->cond);

    printf ("pop r%d\n", r);
    printf ("and r%d, r%d\n", r, r);
    printf ("jne %s\n", elselbl);

    handle (state, condstate->body);

    printf ("jmp %s\n", endlbl);
    printf (".%s\n", elselbl);

    handle (state, condstate->elsebody);

    printf (".%s\n", endlbl);

    popreg (state);
}

static void hfloop (emitstate_t * state, astnode_t * node) {}

static void hfunccall (emitstate_t * state, astnode_t * node) {
    funccallstate_t * funccallstate = (funccallstate_t *)node->state;
    
    int r = pushreg (state);

    size_t argc = llist_size (funccallstate->args);
    for (int i = argc - 1; i >= 0; i--) {
        handle (state, (astnode_t *)llist_get (funccallstate->args, i));
    }

    handle (state, funccallstate->target);
    
    printf ("pop r%d\n", r);
    printf ("call r%d\n", r);

    for (int i = 0; i < argc; i++) {
        printf ("pop r%d\n", r);
    }

    popreg (state);
}

static void hfuncdec (emitstate_t * state, astnode_t * node) {
    funcdecstate_t * funcdecstate = (funcdecstate_t *)node->state;

    printf (".%s\n", funcdecstate->name);
    printf ("push bp\nld bp, sp\n");

    handle (state, funcdecstate->body);
}

static void hid (emitstate_t * state, astnode_t * node) {}
static void hintc (emitstate_t * state, astnode_t * node) {}
static void hret (emitstate_t * state, astnode_t * node) {}

static void hstringc (emitstate_t * state, astnode_t * node) {
    stringcstate_t * stringcstate = (stringcstate_t *)node->state;

    char * sym = gensym (state);
    state->lblkeys = llist_add (state->lblkeys, sym);
    state->lblvals = llist_add (state->lblvals, (void *)stringcstate->s);

    printf ("push %s\n", sym);
}

static void hsubscr (emitstate_t * state, astnode_t * node) {}
static void huop (emitstate_t * state, astnode_t * node) {}
static void hwloop (emitstate_t * state, astnode_t * node) {}

static void handle (emitstate_t * state, astnode_t * node) {
    if (node == NULL) return;
    switch (node->type) {
    case assignnode:
        hassign (state, node);
        break;
    case binopnode:
        hbinop (state, node);
        break;
    case blocknode:
        hblock (state, node);
        break;
    case charcnode:
        hcharc (state, node);
        break;
    case condnode:
        hcond (state, node);
        break;
    case floopnode:
        hfloop (state, node);
        break;
    case funccallnode:
        hfunccall (state, node);
        break;
    case funcdecnode:
        hfuncdec (state, node);
        break;
    case idnode:
        hid (state, node);
        break;
    case intcnode:
        hintc (state, node);
        break;
    case retnode:
        hret (state, node);
        break;
    case stringcnode:
        hstringc (state, node);
        break;
    case subscrnode:
        hsubscr (state, node);
        break;
    case uopnode:
        huop (state, node);
        break;
    case wloopnode:
        hwloop (state, node);
        break;
    }
}

static int pushreg (emitstate_t * state) {
    return state->curreg++;
}

static int popreg (emitstate_t * state) {
    return --state->curreg;
}

static char * gensym (emitstate_t * state) {
    char * buff = (char *)calloc (1, 100);
    sprintf (buff, "sym%d", state->cursym++);
    return buff;
}
