#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/lexerstate.h>
#include <lib/token.h>
#include <lib/toktype.h>

int main (int argc, char * argv[]) {
    FILE * f = fopen (argv [1], "r");
    lexerstate_t * lexer = lexer_init (f);
    
    lexer_destruct (lexer);    

    fclose (f);
}
