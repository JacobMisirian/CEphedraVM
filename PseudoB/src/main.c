#include <inc/lexer.h>
#include <inc/llist.h>
#include <inc/parser.h>
#include <inc/token.h>
#include <inc/toktype.h>

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
    FILE * in = fopen (argv [1], "r");
    lexerstate_t * lexer = lexer_init (in);
    fclose (in);

    token_t * tok = (token_t *)malloc (sizeof (token_t));
    do {
        lexer_nexttok (lexer, tok);
        printf ("Type: %d, Val= \"%s\"\n", tok->type, tok->val);
    } while (tok->type != eof);

    lexer_spos (lexer, 0);


    parserstate_t * parser = parser_init (lexer);

    parser_parse (parser);

    size_t size = llist_size (parser->children);
    for (int i = 0; i < size; i++) {
        printf ("Child: %d\n", ((astnode_t *)llist_get (parser->children, i))->type);
    }

    lexer_free (lexer);
}
