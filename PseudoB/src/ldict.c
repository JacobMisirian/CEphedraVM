#include <inc/ldict.h>

ldictstate_t * ldict_init () {
    ldictstate_t * state = (ldictstate_t *)malloc (sizeof (ldictstate_t));
    state->keys = NULL;
    state->vals = NULL;

    return state;
}

void ldict_free (ldictstate_t * state) {
    llist_free (state->keys);
    llist_free (state->vals);
}

void ldict_add (ldictstate_t * state, void * key, void * val) {
    state->keys = llist_add (state->keys, key);
    state->vals = llist_add (state->vals, val);
}

void * ldict_getv (ldictstate_t * state, void * key) {
    size_t size = ldict_size (state);

    for (int i = 0; i < size; i++) {
        if (llist_get (state->keys, i) == key) {
            return llist_get (state->vals, i);
        }
    }

    return NULL;
}

void * ldict_getk (ldictstate_t * state, void * val) {
    size_t size = ldict_size (state);

    for (int i = 0; i < size; i++) {
        if (llist_get (state->vals, i) == val) {
            return llist_get (state->keys, i);
        }
    }

    return NULL;
}

size_t ldict_size (ldictstate_t * state) {
    return llist_size (state->keys);
}
