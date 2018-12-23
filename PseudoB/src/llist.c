#include <lib/llist.h>

node_t * llist_init (void * ptr) {
    node_t * root = (node_t *)malloc (sizeof (node_t));
    root->ptr = ptr;
    return root;
}

void llist_free (node_t * root) {
    while (root) {
        node_t * tmp = root;
        root = root->next;
        free (tmp);
    }
}

void llist_add (node_t * root, void * ptr) {
    while (root->next != NULL) root = root->next;
    root->next = (node_t *)malloc (sizeof (node_t));
    root->next->ptr = ptr;
}

void * llist_get (node_t * root, int i) {
    for (int j = 0; (j < i) && root != NULL; j++) {
        root = root->next;
    }
    return root->ptr;
}

int llist_size (node_t * root) {
    if (root == NULL) return 0;
    int size = 1;
    while (root->next != NULL) {
        size++;
    }
    return size;
}
