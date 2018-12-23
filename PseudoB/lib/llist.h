#ifndef _LLIST_H_
#define _LLIST_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct node {
    void * ptr;
    struct node * next;
} node_t;

node_t * llist_init (void * ptr);
void llist_free (node_t * root);
void * llist_get (node_t * root, int i);
size_t llist_size (node_t * root);

#endif
