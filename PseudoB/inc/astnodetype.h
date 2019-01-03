#ifndef _AST_NODE_TYPE_H_
#define _AST_NODE_TYPE_H_

typedef enum {
    assignnode, binopnode, blocknode, breaknode, charcnode, condnode, contnode, derefnode, dummynode, expstmtnode, floopnode, funccallnode, funcdecnode, idnode, intcnode, refnode, retnode, stringcnode, subscrnode, wloopnode
} astnodetype_t;

#endif
