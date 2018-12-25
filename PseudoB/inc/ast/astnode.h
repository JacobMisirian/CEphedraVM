#ifndef _AST_NODE_H_
#define _AST_NODE_H_

#include <inc/ast/astnodetype.h>
#include <inc/ast/binoptype.h>
#include <inc/llist.h>
#include <stdlib.h>

void astnode_free (astnode_t * node);
astnode_t * assignnode_init (astnode_t * left, astnode_t * right);
astnode_t * binopnode_init (binoptype_t type, astnode_t * left, astnode_t * right);
astnode_t * charcnode_init (char c);
astnode_t * condnode_init (astnode_t * cond, astnode_t * body, astnode_t * elsebody);
astnode_t * floopnode_init (astnode_t * prestmt, astnode_t * cond, astnode_t * repstmt, astnode_t * body);
astnode_t * intcnode_init (int i);

typedef node_t * llist;

typedef struct {
    astnodetype_t type;
    void * state;
    llist children;
} astnode_t;

typedef struct {
    astnode_t * left;
    astnode_t * right;
} assignstate_t;

typedef struct {
    binoptype_t type;
    astnode_t * left;
    astnode_t * value;
} binopstate_t;

typedef struct {
    char c;
} charcstate_t;

typedef struct {
    astnode_t * cond;
    astnode_t * body;
    astnode_t * elsebody;
} condstate_t;

typedef struct {
    astnode_t * prestmt;
    astnode_t * cond;
    astnode_t * repstmt;
    astnode_t * body;
} floopstate_t;

typedef struct {
    int i;
} intcstate_t;

#endif
