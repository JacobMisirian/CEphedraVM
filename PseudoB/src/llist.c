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
    node_t * tmp = root;
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
    return tmp;
}

node_t * llist_remove (node_t * root, int i) {
    node_t * tmp = root;
    if (root == NULL) {
        return NULL;
    }
    if (i == 0) {
        free (root);
        return NULL;
    }

    node_t * pre = root;
    size_t size = llist_size (root);
    for (int j = 0; (j < i) && (root != NULL); j++) {
        pre = root;
        root = root->next;
    }

    pre->next = root->next;
    free (root);

    return tmp;
}

void * llist_get (node_t * root, int i) {
    for (int j = 0; (j < i) && (root != NULL); j++) {
        root = root->next;
    }
    return root->ptr;
}

int llist_index (node_t * root, const char * ptr) {
    size_t size = llist_size (root);
    for (int i = 0; i < size; i++) {
        if (strcmp ((const char *)llist_get (root, i), ptr) == 0) {
            return i;
        }
    }
    return -1;
}
