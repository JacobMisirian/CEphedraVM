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
}
