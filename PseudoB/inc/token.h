#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <inc/toktype.h>

typedef struct {
    char * val;
    toktype_t type;
} token_t;

#endif
