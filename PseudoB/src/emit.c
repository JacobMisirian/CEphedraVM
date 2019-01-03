#include <inc/emit.h>

static void handle (emitstate_t *, astnode_t *);
static void hassign (emitstate_t *, astnode_t *);
static void hbinop (emitstate_t *, astnode_t *);
static void hblock (emitstate_t *, astnode_t *);
static void hbreak (emitstate_t *, astnode_t *);
static void hcharc (emitstate_t *, astnode_t *);
static void hcond (emitstate_t *, astnode_t *);
static void hcont (emitstate_t *, astnode_t *);
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
static int peekreg (emitstate_t *);
static char * gensym (emitstate_t *);

emitstate_t * emit_init (parserstate_t * parser) {
    emitstate_t * state = (emitstate_t *)malloc (sizeof (emitstate_t));
    state->funcs = parser->funcs;
    state->curreg = 0;
    state->lbls = ldict_init ();
    state->breakstack = lstack_init ();
    state->contstack = lstack_init ();

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

    int r0, r1, r2;

    if (assignstate->left->type == derefnode) {
        handle (state, ((derefstate_t *)assignstate->left->state)->target);
        handle (state, assignstate->right);
        r1 = popreg (state);
        r0 = popreg (state);

        printf ("sw r%d, r%d\n", r0, r1);
    }
    else if (assignstate->left->type == idnode) {
        const char * ident = ((idstate_t *)assignstate->left->state)->id;
        r0 = pushreg (state);

        printf ("ld r%d, bp\n", r0);
        
        int argi = getargi (state, ident);
        if (argi == -1) {
            printf ("sub r%d, %d\n", r0, getlocali (state, ident));
        }
        else {
            printf ("add r%d, %d\n", r0, argi);
        }

        handle (state, assignstate->right);
        r1 = popreg (state);

        printf ("sw r%d, r%d\n", r0, r1);
        popreg (state); // pop r0
    }
    else if (assignstate->left->type == subscrnode) {
        subscrstate_t * subscrstate = (subscrstate_t *)assignstate->left->state;
        handle (state, subscrstate->val);
        handle (state, subscrstate->target);
        handle (state, assignstate->right);
        int r2 = popreg (state);
        r0 = popreg (state);
        r1 = popreg (state);

        printf ("add r%d, r%d\n", r0, r1);
        
        printf ("sb r%d, r%d\n", r0, r2);
    }
}

static void hbinop (emitstate_t * state, astnode_t * node) {
    binopstate_t * binopstate = (binopstate_t *)node->state;

    int r0, r1;

    handle (state, binopstate->left);   
    handle (state, binopstate->right);
    r1 = popreg (state);
    r0 = popreg (state);

    switch (binopstate->type) {
    case add:
        printf ("add r%d, r%d\n", r0, r1);
        pushreg (state);
        break;
    case and:
        printf ("and r%d, r%d\n", r0, r1);
        pushreg (state);
        break;
    case eq:
        printf ("sub r%d, r%d\n", r0, r1);
        printf ("ld r%d, flags\n", r0);
        printf ("and r%d, 1\n", r0);
        pushreg (state);
        break;
    case divide:
        printf ("div r%d, r%d\n", r0, r1);
        pushreg (state);
        break;
    case great:
        printf ("sub r%d, r%d\n", r1, r0);
        printf ("ld r%d, flags\n", r0);
        printf ("shir r%d, 1\n", r0);
        pushreg (state);
        break;
    case greateq:
        printf ("sub r%d, r%d\n", r1, r0);
        printf ("ld r%d, flags\n", r0);
        printf ("and r%d, 3\n", r0);
        pushreg (state);
        break;
    case less:
        printf ("sub r%d, r%d\n", r0, r1);
        printf ("ld r%d, flags\n", r0);
        printf ("shir r%d, 1\n", r0);
        pushreg (state);
        break;
    case lesseq:
        printf ("sub r%d, r%d\n", r0, r1);
        printf ("ld r%d, flags\n", r0);
        printf ("and r%d, 3\n", r0);
        pushreg (state);
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

static void hbreak (emitstate_t * state, astnode_t * node) {
    char * endsym = lstack_peek (state->breakstack);

    printf ("jmp %s\n", endsym);
}

static void hcharc (emitstate_t * state, astnode_t * node) {
    charcstate_t * charcstate = (charcstate_t *)node->state;

    printf ("ld r%d, %d\n", pushreg (state), (int)charcstate->c);
}

static void hcond (emitstate_t * state, astnode_t * node) {
    condstate_t * condstate = (condstate_t *)node->state;

    int r;

    char * elselbl = gensym (state);
    char * endlbl = gensym (state);

    handle (state, condstate->cond);
    r = popreg (state);

    printf ("sub r%d, 1\n", r);
    printf ("jne %s\n", elselbl);

    handle (state, condstate->body);

    printf ("jmp %s\n", endlbl);
    printf (".%s\n", elselbl);

    handle (state, condstate->elsebody);

    printf (".%s\n", endlbl);
}

static void hcont (emitstate_t * state, astnode_t * node) {
    char * loopsym = (char *)lstack_peek (state->contstack);

    printf ("jmp %s\n", loopsym);
}

static void hderef (emitstate_t * state, astnode_t * node) {
    derefstate_t * derefstate = (derefstate_t *)node->state;

    int r;

    handle (state, derefstate->target);
    r = popreg (state);

    printf ("lw r%d, r%d\n", r, r);
    pushreg (state);
}

static void hfloop (emitstate_t * state, astnode_t * node) {
    floopstate_t * floopstate = (floopstate_t *)node->state;

    char * loopsym = gensym (state);
    char * endsym = gensym (state);

    lstack_push (state->breakstack, (void *)endsym);
    lstack_push (state->contstack, (void *)loopsym);

    handle (state, floopstate->prestmt);

    printf (".%s\n", loopsym);

    handle (state, floopstate->cond);
    int r = popreg (state);

    printf ("sub r%d, 1\n", r);
    printf ("jne %s\n", endsym);

    handle (state, floopstate->body);
    handle (state, floopstate->repstmt);

    printf ("jmp %s\n", loopsym);
    printf (".%s\n", endsym);

    lstack_pop (state->breakstack);
    lstack_pop (state->contstack);
}

static void hfunccall (emitstate_t * state, astnode_t * node) {
    funccallstate_t * funccallstate = (funccallstate_t *)node->state;
    
    int r0, r1;

    size_t argc = llist_size (funccallstate->args);
    for (int i = argc - 1; i >= 0; i--) {
        astnode_t * arg = (astnode_t *)llist_get (funccallstate->args, i);
        if (arg != NULL) {
            handle (state, arg);
            r0 = popreg (state);
            printf ("push r%d\n", r0);
        }
    }

    handle (state, funccallstate->target);
    r1 = popreg (state);

    printf ("call r%d\n", r1);

    for (int i = 0; i < argc - 1; i++) {
        printf ("pop r%d\n", r1);
    }
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
}

static void hintc (emitstate_t * state, astnode_t * node) {
    intcstate_t * intcstate = (intcstate_t *)node->state;

    printf ("ld r%d, %d\n", pushreg (state), intcstate->i);
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
}

static void hret (emitstate_t * state, astnode_t * node) {}

static void hstringc (emitstate_t * state, astnode_t * node) {
    stringcstate_t * stringcstate = (stringcstate_t *)node->state;

    char * sym = ldict_getk (state->lbls, (void *)stringcstate->s);
    if (sym == NULL) {
        sym = gensym (state);
        ldict_add (state->lbls, sym, (void *)stringcstate->s);
    }

    printf ("ld r%d, %s\n", pushreg (state), sym);
}

static void hsubscr (emitstate_t * state, astnode_t * node) {
    subscrstate_t * subscrstate = (subscrstate_t *)node->state;

    int r0, r1;

    handle (state, subscrstate->target);
    handle (state, subscrstate->val);
    r1 = popreg (state);
    r0 = popreg (state);

    printf ("add r%d, r%d\n", r0, r1);
    printf ("lb r%d, r%d\n", r0, r0);

    pushreg (state);
}

static void hwloop (emitstate_t * state, astnode_t * node) {
    wloopstate_t * wloopstate = (wloopstate_t *)node->state;

    char * loopsym = gensym (state);
    char * endsym = gensym (state);

    printf (".%s\n", loopsym);

    handle (state, wloopstate->cond);
    int r = popreg (state);

    printf ("sub r%d, 1\n", r);
    printf ("jne %s\n", endsym);

    handle (state, wloopstate->body);

    printf ("jmp %s\n", loopsym);
    printf (".%s\n", endsym);
}

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
    case breaknode:
        hbreak (state, node);
        break;
    case charcnode:
        hcharc (state, node);
        break;
    case condnode:
        hcond (state, node);
        break;
    case contnode:
        hcont (state, node);
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

static int peekreg (emitstate_t * state) {
    return state->curreg;
}

static char * gensym (emitstate_t * state) {
    char * buff = (char *)calloc (1, 100);
    sprintf (buff, "sym%d", state->cursym++);
    return buff;
}
