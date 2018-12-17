#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/lexer.h>
#include <lib/token.h>
#include <lib/toktype.h>

int main (int argc, char * argv[]) {
    FILE * f = fopen (argv [1], "r");
    lexerstate_t * lexer = lexer_init (f);

    token_t * tok = (token_t *)malloc (sizeof (token_t));
    do {
        lexer_nexttok (lexer, tok);
        printf ("Type: %d, Val= \"%s\"\n", tok->type, tok->val);
    } while (tok->type != eof);

    lexer_destruct (lexer);
    free (tok);
    fclose (f);
}
