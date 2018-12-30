#ifndef _AST_NODE_H_
#define _AST_NODE_H_

#include <inc/astnodetype.h>
#include <inc/binoptype.h>
#include <inc/llist.h>
#include <stdlib.h>

typedef struct {
    astnodetype_t type;
    void * state;
} astnode_t;

typedef struct {
    astnode_t * left;
    astnode_t * right;
} assignstate_t;

typedef struct {
    binoptype_t type;
    astnode_t * left;
    astnode_t * right;
} binopstate_t;

typedef struct {
    llist members;
} blockstate_t;

typedef struct {
    char c;
} charcstate_t;

typedef struct {
    astnode_t * cond;
    astnode_t * body;
    astnode_t * elsebody;
} condstate_t;

typedef struct {
    astnode_t * target;
} derefstate_t;

typedef struct {
    astnode_t * prestmt;
    astnode_t * cond;
    astnode_t * repstmt;
    astnode_t * body;
} floopstate_t;

typedef struct {
    astnode_t * target;
    llist args;
} funccallstate_t;

typedef struct {
    const char * name;
    llist args;
    astnode_t * body;
    llist locals;
} funcdecstate_t;

typedef struct {
    const char * id;
} idstate_t;

typedef struct {
    int i;
} intcstate_t;

typedef struct {
    const char * id;
} refstate_t;

typedef struct {
    astnode_t * val;
} retstate_t;

typedef struct {
    const char * s;
} stringcstate_t;

typedef struct {
    astnode_t * target;
    astnode_t * val;
} subscrstate_t;

typedef struct {
    astnode_t * cond;
    astnode_t * body;
} wloopstate_t;

void astnode_free (astnode_t * node);
astnode_t * assignnode_init (astnode_t * left, astnode_t * right);
astnode_t * binopnode_init (binoptype_t type, astnode_t * left, astnode_t * right);
astnode_t * blocknode_init (llist l);
astnode_t * charcnode_init (char c);
astnode_t * condnode_init (astnode_t * cond, astnode_t * body, astnode_t * elsebody);
astnode_t * derefnode_init (astnode_t * target);
astnode_t * floopnode_init (astnode_t * prestmt, astnode_t * cond, astnode_t * repstmt, astnode_t * body);
astnode_t * funccallnode_init (astnode_t * target, llist args);
astnode_t * funcdecnode_init (const char * name, llist args, astnode_t * body, llist locals);
astnode_t * idnode_init (const char * id);
astnode_t * intcnode_init (int i);
astnode_t * refnode_init (const char * id);
astnode_t * retnode_init (astnode_t * val);
astnode_t * stringcnode_init (const char * s);
astnode_t * subscrnode_init (astnode_t * target, astnode_t * val);
astnode_t * wloopnode_init (astnode_t * cond, astnode_t * body);

#endif
