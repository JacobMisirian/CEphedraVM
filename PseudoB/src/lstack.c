#include <inc/lstack.h>

lstackstate_t * lstack_init () {
    lstackstate_t * state = (lstackstate_t *)malloc (sizeof (lstackstate_t));

    state->list = NULL;
    state->top = 0;

    return state;
}

void lstack_free (lstackstate_t * state) {
    if (state->list != NULL) {
        free (state->list);
    }
    free (state);
}

void * lstack_peek (lstackstate_t * state) {
    if (lstack_empty (state)) {
        return NULL;
    }

    return llist_get (state->list, state->top - 1);
}

void * lstack_pop (lstackstate_t * state) {
    if (lstack_empty (state)) {
        return NULL;
    }

    void * ret = llist_get (state->list, --state->top);
    llist_remove (state->list, state->top);

    return ret;
}

void lstack_push (lstackstate_t * state, void * ptr) {
    state->list = llist_add (state->list, ptr);

    state->top++;
}

int lstack_empty (lstackstate_t * state) {
    return state->top == 0;
}
