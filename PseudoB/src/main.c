#include <lib/lexer.h>
#include <lib/llist.h>
#include <lib/token.h>
#include <lib/toktype.h>

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
    FILE * in = fopen (argv [1], "r");
    lexerstate_t * lexer = lexer_init (in);

    token_t * tok = (token_t *)malloc (sizeof (token_t));
    do {
        lexer_nexttok (lexer, tok);
        printf ("Type: %d, Val= \"%s\"\n", tok->type, tok->val);
    } while (tok->type != eof);

    fclose (in);
    lexer_free (lexer);

    int * a, * b, * c;
    a = (int *)malloc (sizeof (int));
    b = (int *)malloc (sizeof (int));
    c = (int *)malloc (sizeof (int));
    *a = 69;
    *b = 420;
    *c = 1337;

    node_t * root = llist_init (&a);
    printf ("llist size = %d\n", llist_size (root));
    printf ("I am about to call llist_add\n");
    llist_add (&c);
    printf ("llist size = %d\n", llist_size (root));
    llist_add (&b);
    for (int i = 0; i < 3; i++) 
        printf ("llist [%d] = %d\n", i, *(int *)llist_get (root, i));
}
