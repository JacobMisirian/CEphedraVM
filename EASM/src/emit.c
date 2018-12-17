#include <lib/emit.h>

struct emitstate {
    uint16_t pos;
    uint16_t len;
    lexerstate_t * lexer;
    token_t * tok;
};

static uint32_t expectinst (struct emitstate * state);
static int expectcomma (struct emitstate * state);
static int expectreg   (struct emitstate * state);
static int expectimm   (struct emitstate * state);
static int seeklabel (struct emitstate * state, const char * lbl);
static int binarylen (struct emitstate * state);

int assemble (lexerstate_t * lexer, uint8_t * bin) {
    struct emitstate * state = (struct emitstate *)malloc (sizeof (struct emitstate));
    state->lexer = lexer;
    state->pos = 0;
    state->tok = (token_t *)malloc (sizeof (token_t));
    state->len = binarylen (state);

    bin = (uint8_t *)malloc (state->len);

    lexer_nexttok (lexer, state->tok);
    while (state->tok->type != eof) {
        if (state->tok->type == string) {
            int len = strlen (state->tok->val);
            for (int i = 0; i < len; i++) {
                bin [state->pos++] = state->tok->val [i];
            }
            bin [state->pos++] = 0;
        }
        else if (state->tok->type == instruction) {
            uint32_t i = expectinst (state);
            bin [state->pos++] = (i >> 24) & 0xFF;
            bin [state->pos++] = (i >> 16) & 0xFF;
            bin [state->pos++] = (i >>  8) & 0xFF;
            bin [state->pos++] = i & 0xFF;
        }
        lexer_nexttok (lexer, state->tok);
    }

    int binsize = state->pos;

    free (state->tok);
    free (state);

    return binsize;
}

static uint32_t expectinst (struct emitstate * state) {
    uint8_t code = 0, op1 = 0, op2 = 0;
    uint16_t imm = 0;

    char * id = state->tok->val;
    code = getinst (id);
    // <op> <r1>, [<r2>/<int>/<lbl>]
    if (strcmp (id, "add") == 0 || strcmp (id, "lb")  == 0 || 
        strcmp (id, "lw")  == 0 || strcmp (id, "mod") == 0 ||
        strcmp (id, "sb")  == 0 || strcmp (id, "sw")  == 0 ||
        strcmp (id, "sub") == 0) {
        op1 = expectreg (state);
        expectcomma (state);
	int lexertmp = lexer_gpos (state->lexer);
        lexer_nexttok (state->lexer, state->tok);
        lexer_spos (state->lexer, lexertmp);
        if (state->tok->type == reg) {
            op2 = expectreg (state);
        }
        else {
            code = getinst (strcat (id, "i"));
            imm = expectimm (state);
        }
    }
    // <op> [<int>/<lbl>]
    else if (strcmp (id, "jmp") == 0) {
        imm = expectimm (state);
    }
    // <op> <r1>, [<int>, <lbl>]
    else if (strcmp (id, "li") == 0) {
        op1 = expectreg (state);
        expectcomma (state);
        imm = expectimm (state);
    }
    // <op> <r1>, <r2>
    else if (strcmp (id, "mov") == 0) {
        op1 = expectreg (state);
        expectcomma (state);
        op2 = expectreg (state);
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
            code = getinst (strcat (id, "i"));
            imm = expectimm (state);
        }
    }
    return buildinst (code, op1, op2, imm);
}

static int expectcomma (struct emitstate * state) {
    lexer_nexttok (state->lexer, state->tok);
    if (state->tok->type != comma) {
        printf ("Expected comma, got %d with val: \"%s\"\n", state->tok->type, state->tok->val);
        return -1;
    }
    return 1;
}

static int expectreg (struct emitstate * state) {
    lexer_nexttok (state->lexer, state->tok);
    if (state->tok->type != reg) {
        printf ("Expected register, got %d with val: \"%s\"\n", state->tok->type, state->tok->val);
        return -1;
    }
    
    return getregister (state->tok->val);    
}

static int expectimm (struct emitstate * state) {
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

static int seeklabel (struct emitstate * state, const char * lbl) {
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

static int binarylen (struct emitstate * state) {
    int len = 0;
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
