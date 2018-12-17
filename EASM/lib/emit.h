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

uint8_t * assemble (lexerstate_t * lexer);

#endif
