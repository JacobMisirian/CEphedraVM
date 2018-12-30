#ifndef _EMIT_H_
#define _EMIT_H_

#include <inc/astnode.h>
#include <inc/astnodetype.h>
#include <inc/binoptype.h>
#include <inc/ldict.h>
#include <inc/llist.h>
#include <inc/parser.h>
#include <stdlib.h>

typedef struct {
    llist funcs;
    funcdecstate_t * curfunc;
    ldictstate_t * lbls;
    int curreg;
    int cursym;
} emitstate_t;

emitstate_t * emit_init (parserstate_t * parser);
void emit_free (emitstate_t * state);
void emit_run (emitstate_t * state);

#endif
