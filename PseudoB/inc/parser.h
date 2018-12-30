#ifndef _PARSER_H_
#define _PARSER_H_

#include <inc/astnode.h>
#include <inc/astnodetype.h>
#include <inc/ldict.h>
#include <inc/lexer.h>
#include <inc/llist.h>
#include <string.h>
#include <inc/token.h>
#include <inc/toktype.h>

typedef struct {
    lexerstate_t * lexer;
    node_t * children;
    token_t * tok;
    llist locals;
} parserstate_t;

parserstate_t * parser_init (lexerstate_t * lexer);
void parser_free (parserstate_t * state);
void parser_parse (parserstate_t * state);

#endif
