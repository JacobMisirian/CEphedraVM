#include <lib/emit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/lexer.h>
#include <lib/token.h>
#include <lib/toktype.h>

int main (int argc, char * argv[]) {
    FILE * in = fopen (argv [1], "r");
    lexerstate_t * lexer = lexer_init (in);

    token_t * tok = (token_t *)malloc (sizeof (token_t));
    do {
        lexer_nexttok (lexer, tok);
        printf ("Type: %d, Val= \"%s\"\n", tok->type, tok->val);
    } while (tok->type != eof);

    lexer_spos (lexer, 0);
    uint8_t * bin;
    int binsize = assemble (lexer, bin);

    lexer_destruct (lexer);
    fclose (in);

    FILE * out = fopen (argv [2], "w");
    fwrite (bin, 1, binsize, out);
    fflush (out);
    fclose (out);
}
