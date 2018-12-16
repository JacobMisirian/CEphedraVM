#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/lexer.h>
#include <lib/token.h>
#include <lib/toktype.h>

int main (int argc, char * argv[]) {
    FILE * f = fopen (argv [1], "r");
    lexerstate_t * lexer = lexer_init (f);

    token_t * tok = lexer_nextstr (lexer);

    printf ("Token type %d, token value: \"%s\"\n", tok->type, tok->val);
    
    tok = lexer_nextstr (lexer);
    
    printf ("Token type %d, token value: \"%s\"\n", tok->type, tok->val);

    for (int i = 0; i < 2; i++) {
        tok = lexer_nextident (lexer);
        printf ("Token type %d, token value: \"%s\"\n", tok->type, tok->val);
    }

    lexer_destruct (lexer);    

    fclose (f);
}