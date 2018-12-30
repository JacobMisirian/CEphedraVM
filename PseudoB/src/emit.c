#include <inc/emit.h>

static void handle (emitstate_t *, astnode_t *);
static void hassign (emitstate_t *, astnode_t *);
static void hbinop (emitstate_t *, astnode_t *);
static void hblock (emitstate_t *, astnode_t *);
static void hcharc (emitstate_t *, astnode_t *);
static void hcond (emitstate_t *, astnode_t *);
static void hderef (emitstate_t *, astnode_t *);
static void hfloop (emitstate_t *, astnode_t *);
static void hfunccall (emitstate_t *, astnode_t *);
static void hfuncdec (emitstate_t *, astnode_t *);
static void hid (emitstate_t *, astnode_t *);
static void hintc (emitstate_t *, astnode_t *);
static void href (emitstate_t *, astnode_t *);
static void hret (emitstate_t *, astnode_t *);
static void hstringc (emitstate_t *, astnode_t *);
static void hsubscr (emitstate_t *, astnode_t *);
static void hwloop (emitstate_t *, astnode_t *);
static int getargi (emitstate_t *, const char *);
static int getlocali (emitstate_t *, const char *);
static int pushreg (emitstate_t *);
static int popreg (emitstate_t *);
static char * gensym (emitstate_t *);

emitstate_t * emit_init (parserstate_t * parser) {
    emitstate_t * state = (emitstate_t *)malloc (sizeof (emitstate_t));
    state->funcs = parser->funcs;
    state->curreg = 0;
    state->lbls = ldict_init ();

    return state;
}

void emit_free (emitstate_t * state) {
    ldict_free (state->lbls);
    free (state);
}

void emit_run (emitstate_t * state) {
    printf ("ld sp, 2000\n");
    printf ("call main\n");
    printf ("hcf\n");

    size_t clen = llist_size (state->funcs);
    for (int i = 0; i < clen; i++) {
        astnode_t * node = (astnode_t *)llist_get (state->funcs, i);
        handle (state, node);
    }

    size_t llen = ldict_size (state->lbls);

    for (int i = 0; i < llen; i++) {
        const char * key = (const char *)llist_get (state->lbls->keys, i);
        const char * val = (const char *)ldict_getv (state->lbls, (void *)key);
        printf (".%s\n", key);
        printf ("\"%s\"\n", val);
    }
}

static void hassign (emitstate_t * state, astnode_t * node) {
    assignstate_t * assignstate = (assignstate_t *)node->state;

    int r0 = pushreg (state);
    int r1 = pushreg (state);

    if (assignstate->left->type == derefnode) {
        handle (state, ((derefstate_t *)assignstate->left->state)->target);
        printf ("pop r%d\n", r0);
    }
    else if (assignstate->left->type == idnode) {
        const char * ident = ((idstate_t *)assignstate->left->state)->id;
        printf ("ld r%d, bp\n", r0);

        int argi = getargi (state, ident);
        if (argi == -1) {
            printf ("sub r%d, %d\n", r0, getlocali (state, ident));
        }
        else {
            printf ("add r%d, %d\n", r0, argi);
        }
    }   
    
    handle (state, assignstate->right);
        
    printf ("pop r%d\n", r1);
    printf ("sw r%d, r%d\n", r0, r1);

    popreg (state);
    popreg (state);
}

static void hbinop (emitstate_t * state, astnode_t * node) {
    binopstate_t * binopstate = (binopstate_t *)node->state;

    int r0 = pushreg (state);
    int r1 = pushreg (state);

    handle (state, binopstate->right);
    handle (state, binopstate->left);

    printf ("pop r%d\n", r0);
    printf ("pop r%d\n", r1);

    switch (binopstate->type) {
    case less:
        printf ("sub r%d, r%d\n", r0, r1);
        printf ("shir flags, 1\n");
        printf ("push flags\n");
        break;
    }

    popreg (state);
    popreg (state);
    printf ("\n");
}

static void hblock (emitstate_t * state, astnode_t * node) {
    blockstate_t * blockstate = (blockstate_t *)node->state;

    size_t len = llist_size (blockstate->members);
    for (int i = 0; i < len; i++) {
        handle (state, llist_get (blockstate->members, i));
    }
}

static void hcharc (emitstate_t * state, astnode_t * node) {
    charcstate_t * charcstate = (charcstate_t *)node->state;

    printf ("push %d\n", (int)charcstate->c);
}

static void hcond (emitstate_t * state, astnode_t * node) {
    condstate_t * condstate = (condstate_t *)node->state;

    int r = pushreg (state);

    char * elselbl = gensym (state);
    char * endlbl = gensym (state);

    handle (state, condstate->cond);

    printf ("pop r%d\n", r);
    printf ("sub r%d, r%d\n", r, r);
    printf ("jne %s\n", elselbl);

    popreg (state);

    handle (state, condstate->body);

    printf ("jmp %s\n", endlbl);
    printf (".%s\n", elselbl);

    handle (state, condstate->elsebody);

    printf (".%s\n", endlbl);
    printf ("\n");
}

static void hderef (emitstate_t * state, astnode_t * node) {
    
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

    for (int i = 0; i < argc - 1; i++) {
        printf ("pop r%d\n", r);
    }

    popreg (state);
    printf ("\n");
}

static void hfuncdec (emitstate_t * state, astnode_t * node) {
    funcdecstate_t * funcdecstate = (funcdecstate_t *)node->state;
    state->curfunc = funcdecstate;
 
    int localsize = (int)llist_size (state->curfunc->locals) * 2;

    printf (".%s\n", funcdecstate->name);
    printf ("push bp\n");
    printf ("ld bp, sp\n");
    printf ("sub sp, %d\n", localsize);
    
    handle (state, funcdecstate->body);
    
    printf ("add sp, %d\n", localsize);
    printf ("pop bp\n");

    printf ("ret\n");
}

static void hid (emitstate_t * state, astnode_t * node) {
    idstate_t * idstate = (idstate_t *)node->state;

    int r = pushreg (state);

    int argi = getargi (state, idstate->id);
    int locali = getlocali (state, idstate->id);

    if (locali != -1) {
        printf ("ld r%d, bp\n", r);
        printf ("sub r%d, %d\n", r, locali);
        printf ("lw r%d, r%d\n", r, r);
    }
    else if (argi != -1) {
        printf ("ld r%d, bp\n", r);
        printf ("add r%d, %d\n", r, argi);
        printf ("lw r%d, r%d\n", r, r);
    }
    else {
        printf ("ld r%d, %s\n", r, idstate->id);
    }

    printf ("push r%d\n", r);

    popreg (state);
}

static void hintc (emitstate_t * state, astnode_t * node) {
    intcstate_t * intcstate = (intcstate_t *)node->state;

    printf ("push %d\n", intcstate->i);
}

static void href (emitstate_t * state, astnode_t * node) {
    refstate_t * refstate = (refstate_t *)node->state;

    int r = pushreg (state);

    printf ("ld r%d, bp\n", r);

    int argi = getargi (state, refstate->id);
    if (argi == -1) {
        printf ("sub r%d, %d\n", r, getlocali (state, refstate->id));
    }
    else {
        printf ("add r%d, %d\n", r, argi);
    }

    printf ("push r%d\n", r);

    popreg (state);
}

static void hret (emitstate_t * state, astnode_t * node) {}

static void hstringc (emitstate_t * state, astnode_t * node) {
    stringcstate_t * stringcstate = (stringcstate_t *)node->state;

    char * sym = gensym (state);
    ldict_add (state->lbls, sym, (void *)stringcstate->s);

    printf ("push %s\n", sym);
    printf ("\n");
}

static void hsubscr (emitstate_t * state, astnode_t * node) {}

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
    case derefnode:
        hderef (state, node);
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
    case refnode:
        href (state, node);
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
    case wloopnode:
        hwloop (state, node);
        break;
    }
}

static int getargi (emitstate_t * state, const char * id) {
    int i = llist_index (state->curfunc->args, id);   
    if (i == -1) return i;
    return (i + 2) * 2;
}

static int getlocali (emitstate_t * state, const char * id) {
    int i = llist_index (state->curfunc->locals, id);
    if (i == -1) return i;
    return (i + 1) * 2;
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
