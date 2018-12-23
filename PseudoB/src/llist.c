#include <inc/llist.h>

void llist_free (node_t * root) {
    while (root) {
        node_t * tmp = root;
        root = root->next;
        free (tmp);
    }
}

size_t llist_size (node_t * root) {
    size_t size = 0;
    while (root) {
        size++;
        root = root->next;
    }
    return size;
}

node_t * llist_add (node_t * root, void * ptr) {
    if (root == NULL) {
        node_t * ret = (node_t *)calloc (1, sizeof (node_t));
        ret->ptr = ptr;
        return ret;
    }

    while (root->next) {
        root = root->next;
    }

    root->next = (node_t *)calloc (1, sizeof (node_t));
    root->next->ptr = ptr;
    return root->next;
}

void * llist_get (node_t * root, int i) {
    for (int j = 0; (j < i) && (root != NULL); j++) {
        root = root->next;
    }
    return root->ptr;
}
