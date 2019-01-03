#ifndef _LSTACK_H_
#define _LSTACK_H_

#include <inc/llist.h>
#include <stdlib.h>

typedef struct {
    llist list;
    int top;
} lstackstate_t;

lstackstate_t * lstack_init ();
void lstack_free (lstackstate_t * state);
void * lstack_peek (lstackstate_t * state);
void * lstack_pop (lstackstate_t * state);
void lstack_push (lstackstate_t * state, void * ptr);
int lstack_empty (lstackstate_t * state);

#endif
