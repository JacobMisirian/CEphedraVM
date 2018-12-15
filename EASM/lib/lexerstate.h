#ifndef _LEXER_STATE_H_
#define _LEXER_STATE_H_

#include <stdint.h>
#include <lib/token.h>
#include <lib/toktype.h>

typedef struct {
    char * code;
    uint32_t pos;
    uint32_t len;
} lexerstate_t;

#endif
