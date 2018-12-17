#ifndef _TOKTYPE_H_
#define _TOKTYPE_H_

typedef enum {
    comma, eof, error, instruction, integer, labeldec, labelreq, reg, string
} toktype_t;

#endif
