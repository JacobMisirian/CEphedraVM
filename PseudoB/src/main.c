#include <inc/emit.h>
#include <inc/lexer.h>
#include <inc/parser.h>
#include <inc/token.h>
#include <inc/toktype.h>

#include <stdio.h>
#include <stdlib.h>

// #define LEXER_TEST

int main (int argc, char * argv[]) {
    FILE * in = fopen (argv [1], "r");
    lexerstate_t * lexer = lexer_init (in);
    fclose (in);

#ifdef LEXER_TEST
    token_t * tok = (token_t *)calloc (1, sizeof (token_t));
    do {
        lexer_nexttok (lexer, tok);
        printf ("Type: %d, Val= \"%s\"\n", tok->type, tok->val);
    } while (tok->type != eof);
    lexer_spos (lexer, 0);
#endif


    parserstate_t * parser = parser_init (lexer);
    parser_parse (parser);

    emitstate_t * emit = emit_init (parser);
    emit_run (emit);

    parser_free (parser);
    emit_free (emit);
}
