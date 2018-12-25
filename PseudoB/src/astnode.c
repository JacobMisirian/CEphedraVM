#include <inc/astnode.h>

void astnode_free (astnode_t * node) {
    free (node->state);
    free (node);
}

astnode_t * assignnode_init (astnode_t * left, astnode_t * right) {
    assignstate_t * state = (assignstate_t *)malloc (sizeof (assignstate_t));
    state->left = left;
    state->right = right;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = assignnode;

    return node;
}

astnode_t * binopnode_init (binoptype_t type, astnode_t * left, astnode_t * right) {
    binopstate_t * state = (binopstate_t *)malloc (sizeof (binopstate_t));
    state->type = type;
    state->left = left;
    state->right = right;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = binopnode;

    return node;
}

astnode_t * blocknode_init (llist l) {
    blockstate_t * state = (blockstate_t *)malloc (sizeof (blockstate_t));
    state->members = l;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = blocknode;

    return node;
}

astnode_t * charcnode_init (char c) {
    charcstate_t * state = (charcstate_t *)malloc (sizeof (charcstate_t));
    state->c = c;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = charcnode;

    return node;
}

astnode_t * condnode_init (astnode_t * cond_, astnode_t * body, astnode_t * elsebody) {
    condstate_t * state = (condstate_t *)malloc (sizeof (condstate_t));
    state->cond = cond_;
    state->body = body;
    state->elsebody = elsebody;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = condnode;

    return node;
}

astnode_t * floopnode_init (astnode_t * prestmt, astnode_t * cond, astnode_t * repstmt, astnode_t * body) {
    floopstate_t * state = (floopstate_t *)malloc (sizeof (floopstate_t));
    state->prestmt = prestmt;
    state->cond = cond;
    state->repstmt = repstmt;
    state->body = body;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = floopnode;

    return node;
}

astnode_t * intcnode_init (int i) {
    intcstate_t * state = (intcstate_t *)malloc (sizeof (intcstate_t));
    state->i = i;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = intcnode;

    return node;
}

astnode_t * wloopnode_init (astnode_t * cond, astnode_t * body) {
    wloopstate_t * state = (wloopstate_t *)malloc (sizeof (wloopstate_t));
    state->cond = cond;
    state->body = body;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = wloopnode;

    return node;
}
