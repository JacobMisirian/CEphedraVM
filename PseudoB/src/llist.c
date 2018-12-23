#include <lib/llist.h>

node_t * llist_init (void * ptr) {
    node_t * root = (node_t *)malloc (sizeof (node_t));
    root->ptr = ptr;
    root->next = NULL;
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
printf ("Congradulations! C isn't broken\n");
    if (root == NULL) return;
printf ("2\n");
    while (root->next != NULL) {
printf ("3\n");
        if (root == NULL) break;
printf ("4\n");
        root = root->next;
printf ("5\n");
        if (root == NULL) break;
printf ("6\n");
    }
}

void * llist_get (node_t * root, int i) {
    for (int j = 0; (j < i) && root != NULL; j++) {
        root = root->next;
    }
    return root->ptr;
}

size_t llist_size (node_t * root) {
    if (root == NULL) return 0;
    size_t size = 0;
    do {
        size++;
        root = root->next;
    } while (root != NULL);

    return size;
}
