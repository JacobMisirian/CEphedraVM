#ifndef _LLIST_H_
#define _LLIST_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    void * ptr;
    struct node * next;
} node_t;

void llist_free (node_t * root);
size_t llist_size (node_t * root);
node_t * llist_add (node_t * root, void * ptr);
void * llist_get (node_t * root, int i);

#endif
