#ifndef _LLIST_H_
#define _LLIST_H_

#include <stdlib.h>

typedef struct node {
    void * ptr;
    struct node * next;
} node_t;

#endif
