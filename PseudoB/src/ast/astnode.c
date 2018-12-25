#include <inc/ast/astnode.h>

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
    node->type = assign;

    return node;
}

astnode_t * binopnode_init (binoptype_t type, astnode_t * left, astnode_t * right) {
    binopstate_t * state = (binopstate_t *)malloc (sizeof (binopstate_t));
    state->type = type;
    state->left = left;
    state->right = right;

    astnode_t * node = (astnode_t *)malloc (sizeof (astnode_t));
    node->state = state;
    node->type = binop;

    return node;
}
