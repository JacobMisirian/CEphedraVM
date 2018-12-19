#include <lib/emit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/lexer.h>
#include <lib/token.h>
#include <lib/toktype.h>

int main (int argc, char * argv[]) {
    // tokenize source code.
    FILE * in = fopen (argv [1], "r");
    lexerstate_t * lexer = lexer_init (in);
    fclose (in);

    // print tokens.
    token_t * tok = (token_t *)malloc (sizeof (token_t));
    do {
        lexer_nexttok (lexer, tok);
        printf ("Type: %d, Val= \"%s\"\n", tok->type, tok->val);
    } while (tok->type != eof);

    // assemble.
    emitstate_t * emit = emit_init (lexer);
    uint8_t * bin = emit_assemble (emit);

    // write output assembly.
    int len = emit_len (emit);
    FILE * out = fopen (argv [2], "w");
    fwrite (bin, 1, len, out);
    fflush (out);
    fclose (out);

    // clean up memory.
    lexer_destruct (lexer);
    emit_destruct (emit);
    free (tok);

    return 0;
}
