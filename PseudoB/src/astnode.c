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

astnode_t * derefnode_init (astnode_t * target) {
    derefstate_t * state = (derefstate_t *)malloc (sizeof (derefstate_t));
    state->target = target;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = derefnode;

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

astnode_t * funccallnode_init (astnode_t * target, llist args) {
    funccallstate_t * state = (funccallstate_t *)malloc (sizeof (funccallstate_t));
    state->target = target;
    state->args = args;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = funccallnode;

    return node;
}

astnode_t * funcdecnode_init (const char * name, llist args, astnode_t * body, llist locals) {
    funcdecstate_t * state = (funcdecstate_t *)malloc (sizeof (funcdecstate_t));
    state->name = name;
    state->args = args;
    state->body = body;
    state->locals = locals;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = funcdecnode;

    return node;
}

astnode_t * idnode_init (const char * id) {
    idstate_t * state = (idstate_t *)malloc (sizeof (idstate_t));
    state->id = id;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = idnode;

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

astnode_t * refnode_init (const char * id) {
    refstate_t * state = (refstate_t *)malloc (sizeof (refstate_t));
    state->id = id;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = refnode;

    return node;
}

astnode_t * retnode_init (astnode_t * val) {
    retstate_t * state = (retstate_t *)malloc (sizeof (retstate_t));
    state->val = val;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = retnode;

    return node;
}

astnode_t * stringcnode_init (const char * s) {
    stringcstate_t * state = (stringcstate_t *)malloc (sizeof (stringcstate_t));
    state->s = s;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = stringcnode;

    return node;
}

astnode_t * subscrnode_init (astnode_t * target, astnode_t * val) {
    subscrstate_t * state = (subscrstate_t *)malloc (sizeof (subscrstate_t));
    state->target = target;
    state->val = val;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = subscrnode;

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
