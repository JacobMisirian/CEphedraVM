#include <inc/emit.h>

static void handle (astnode_t * node);
static void hassign (astnode_t * node);
static void hbinop (astnode_t * node);
static void hblock (astnode_t * node);
static void hcharc (astnode_t * node);
static void hcond (astnode_t * node);
static void hfloop (astnode_t * node);
static void hfunccall (astnode_t * node);
static void hfuncdec (astnode_t * node);
static void hid (astnode_t * node);
static void hintc (astnode_t * node);
static void hret (astnode_t * node);
static void hstringc (astnode_t * node);
static void hsubscr (astnode_t * node);
static void huop (astnode_t * node);
static void hwloop (astnode_t * node);

emitstate_t * emit_init (parserstate_t * parser) {
    emitstate_t * state = (emitstate_t *)malloc (sizeof (emitstate_t));
    state->ast = parser->children;

    return state;
}

void emit_free (emitstate_t * state) {
    free (state);
}

void emit_run (emitstate_t * state) {
    size_t len = llist_size (state->ast);
    for (int i = 0; i < len; i++) {
        astnode_t * node = (astnode_t *)llist_get (state->ast, i);
        handle (node);
    }
}

static void hassign (astnode_t * node) {}
static void hbinop (astnode_t * node) {}
static void hblock (astnode_t * node) {}
static void hcharc (astnode_t * node) {}
static void hcond (astnode_t * node) {}
static void hfloop (astnode_t * node) {}
static void hfunccall (astnode_t * node) {}
static void hfuncdec (astnode_t * node) {}
static void hid (astnode_t * node) {}
static void hintc (astnode_t * node) {}
static void hret (astnode_t * node) {}
static void hstringc (astnode_t * node) {}
static void hsubscr (astnode_t * node) {}
static void huop (astnode_t * node) {}
static void hwloop (astnode_t * node) {}

static void handle (astnode_t * node) {
    switch (node->type) {
    case assignnode:
        hassign (node);
        break;
    case binopnode:
        hbinop (node);
        break;
    case blocknode:
        hblock (node);
        break;
    case charcnode:
        hcharc (node);
        break;
    case condnode:
        hcond (node);
        break;
    case floopnode:
        hfloop (node);
        break;
    case funccallnode:
        hfunccall (node);
        break;
    case funcdecnode:
        hfuncdec (node);
        break;
    case idnode:
        hid (node);
        break;
    case intcnode:
        hintc (node);
        break;
    case retnode:
        hret (node);
        break;
    case stringcnode:
        hstringc (node);
        break;
    case subscrnode:
        hsubscr (node);
        break;
    case uopnode:
        huop (node);
        break;
    case wloopnode:
        hwloop (node);
        break;
    }
}
