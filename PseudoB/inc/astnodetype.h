#ifndef _AST_NODE_TYPE_H_
#define _AST_NODE_TYPE_H_

typedef enum {
    assignnode, binopnode, blocknode, charcnode, condnode, floopnode, funccallnode, funcdecnode, idnode, intcnode, retnode, stringcnode, subscrnode, uopnode, wloopnode
} astnodetype_t;

#endif
