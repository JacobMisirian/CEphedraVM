#include <inc/emit.h>

static uint32_t expectinst (emitstate_t * state);
static int expectcomma (emitstate_t * state);
static int expectreg   (emitstate_t * state);
static int expectimm   (emitstate_t * state);
static int seeklabel (emitstate_t * state, const char * lbl);
static int binarylen (emitstate_t * state);

emitstate_t * emit_init (lexerstate_t * lexer) {
    emitstate_t * state = (emitstate_t *)malloc (sizeof (emitstate_t)); 
    state->tok = (token_t *)malloc (sizeof (token_t));
    state->lexer = lexer;
    state->pos = 0;
    state->len = binarylen (state);
    state->bin = (uint8_t *)malloc (state->len);

    return state;
}

void emit_destruct (emitstate_t * state) {
    free (state->tok);
    free (state->bin);
    free (state); // Israel!
}

int emit_len (emitstate_t * state) {
    return state->len;
}

uint8_t * emit_assemble (emitstate_t * state) {
    do {
        lexer_nexttok (state->lexer, state->tok); // get next token.
        // if token is a string, encode the literal in the data.
        if (state->tok->type == string) {
            int len = strlen (state->tok->val);
            for (int i = 0; i < len; i++) {
                state->bin [state->pos++] = state->tok->val [i];
            }
            state->bin [state->pos++] = 0;
        }
        // if token is an instruction, encode it.
        else if (state->tok->type == instruction) {
            uint32_t i = expectinst (state);
            unsigned char buffer [4];
            memcpy (buffer, (unsigned char *)&(i), 4);
            for (int i = 0; i < 4; i++)
                state->bin [state->pos++] = buffer[i];
        }
    } while (state->tok->type != eof); // do until eof.
    return state->bin;
}

static uint32_t expectinst (emitstate_t * state) {
    uint8_t code = 0, op1 = 0, op2 = 0;
    uint16_t imm = 0;

    char * id = state->tok->val;
    code = getinst (id);
    // <op> <r1>, [<r2>/<int>/<lbl>]
    if (strcmp (id, "add") == 0 || strcmp (id, "lb") == 0 ||
        strcmp (id, "ld")  == 0 || strcmp (id, "lw") == 0 ||
        strcmp (id, "mod")  == 0 || strcmp (id, "sb")  == 0 ||
        strcmp (id, "sw") == 0 || strcmp (id, "sub") == 0) {
        op1 = expectreg (state);
        expectcomma (state);
        // we need to check 1 token ahead.
	int lexertmp = lexer_gpos (state->lexer);
        lexer_nexttok (state->lexer, state->tok);
        lexer_spos (state->lexer, lexertmp);
        if (state->tok->type == reg) {
            op2 = expectreg (state);
        }
        else {
            op2 |= (1 << 4);
            imm = expectimm (state);
        }
    }
    // <op> [<int>/<lbl>]
    else if (strcmp (id, "jmp") == 0 || strcmp (id, "call") == 0) {
        imm = expectimm (state);
    }
    // <op> <r1>
    else if (strcmp (id, "pop") == 0) {
        op1 = expectreg (state);
    }
    // <op> [<r1>/<int>/<lbl>]
    else if (strcmp (id, "push") == 0) {
        int lexertmp = lexer_gpos (state->lexer);
        lexer_nexttok (state->lexer, state->tok);
        lexer_spos (state->lexer, lexertmp);
        if (state->tok->type == reg) {
            op1 = expectreg (state);
        }
        else {
            op1 |= (1 << 5);
            imm = expectimm (state);
        }
    }
    return buildinst (code, op1, op2, imm);
}

static int expectcomma (emitstate_t * state) {
    lexer_nexttok (state->lexer, state->tok);
    if (state->tok->type != comma) {
        printf ("Expected comma, got %d with val: \"%s\"\n", state->tok->type, state->tok->val);
        return -1;
    }
    return 1;
}

static int expectreg (emitstate_t * state) {
    lexer_nexttok (state->lexer, state->tok);
    if (state->tok->type != reg) {
        printf ("Expected register, got %d with val: \"%s\"\n", state->tok->type, state->tok->val);
        return -1;
    }
    
    return getregister (state->tok->val);    
}

static int expectimm (emitstate_t * state) {
    lexer_nexttok (state->lexer, state->tok);
    if (state->tok->type == integer) {
        return atoi (state->tok->val);
    }
    else if (state->tok->type == labelreq) {
        return seeklabel (state, state->tok->val);
    }

    printf ("Expected int or label request, got %d with val: \"%s\"\n", state->tok->type, state->tok->val);
    return -1;
}

static int seeklabel (emitstate_t * state, const char * lbl) {
    uint16_t realpos = lexer_gpos (state->lexer); // save current lexer position.
    lexer_spos (state->lexer, 0); // rewind lexer to beginning.

    int label = 0; // start counting to the (future) binary position of lbl.
    do {
        lexer_nexttok (state->lexer, state->tok);
        
        if (state->tok->type == instruction) {
            label += INST_SIZE;
        }
        else if (state->tok->type == string) {
            label += strlen (state->tok->val) + 1;
        }
        else if (state->tok->type == labeldec) {
            if (strcmp (state->tok->val, lbl) == 0) {
                lexer_spos (state->lexer, realpos); // restore lexer position.
                return label;
            }
        }
    } while (state->tok->type != eof);

    lexer_spos (state->lexer, realpos); // restore lexer position.

    return -1;
}

static int binarylen (emitstate_t * state) {
    int len = 0;
    lexer_spos (state->lexer, 0);
    do {
        lexer_nexttok (state->lexer, state->tok);
        if (state->tok->type == instruction) {
            len += INST_SIZE;
        }
        else if (state->tok->type == string) {
            len += strlen (state->tok->val) + 1;
        }
    } while (state->tok->type != eof);

    lexer_spos (state->lexer, 0);
    return len;
}
