#ifndef _AST_NODE_TYPE_H_
#define _AST_NODE_TYPE_H_

typedef enum {
    assign, binop, charc, cond, floop, funccall, funcdec, id, intc, stringc, uop, wloop
} astnodetype_t;

#endif
