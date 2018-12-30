#ifndef _LDICT_H_
#define _LDICT_H_

#include <inc/llist.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    llist keys;
    llist vals;
} ldictstate_t;

ldictstate_t * ldict_init ();
void ldict_free (ldictstate_t * state);
void ldict_add (ldictstate_t * state, void * key, void * val);
void * ldict_getv (ldictstate_t * state, void * key);
void * ldict_getk (ldictstate_t * state, void * val);
size_t ldict_size (ldictstate_t * state);

#endif
