#ifndef _EMIT_H_
#define _EMIT_H_

#include <lib/instructions.h>
#include <lib/lexer.h>
#include <lib/registers.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lib/token.h>
#include <lib/toktype.h>

int assemble (lexerstate_t * lexer, uint8_t * bin);

#endif
