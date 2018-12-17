#include <lib/emit.h>

typedef static struct {
    lexerstate_t * lexer;
    uint16_t pos;
    uint16_t len;
    token_t * tok;
} emitcontext_t;

static int expectreg (emitcontext_t * context);
static int expectimm (emitcontext_t * context);
static int seeklabel (emitcontext_t * context, const char * lbl);
static int binarylen (emitcontext_t * context);

uint8_t * assemble (lexerstate_t * lexer) {
    emitcontext_t * context = (emitcontext_t *)malloc (sizeof (emitcontext_t));
    context->lexer = lexer;    
    context->pos = 0;
    context->tok = (token_t *)malloc (sizeof (token_t));
    context->len = binarylen (context);
    
    uint8_t * bin = (uint8_t *)malloc (context->len);

    uint8_t code, op1, op2;
    uint16_t imm;
    lexer_nexttok (lexer, context->tok);
    while (context->tok->type != eof) {
        if (context->tok->type == string) {
            
        }



        lexer_nexttok (lexer, context->tok);
    }

    free (context->tok);
    free (context);
}

static int expectcomma (emitcontext_t * context) {
    lexer_nexttok (context->lexer, context->tok);
    if (context->tok->type != comma) {
        return -1;
    }
    return 1;
}

static int expectreg (emitcontext_t * context) {
    lexer_nexttok (context->lexer, context->tok);
    if (context->tok->type != reg) {
        return -1;
    }
    
    return getregister (context->tok->val);    
}

static int expectimm (emitcontext_t * context) {
    lexer_nexttok (context->lexer, context->tok);
    if (context->tok->type == integer) {
        return atoi (context->tok->val);
    }
    else if (context->tok->type == labelreq) {
        return seeklabel (context, context->tok->val);
    }
    return -1;
}

static int seeklabel (emitcontext_t * context, const char * lbl) {
    uint16_t realpos = lexer_gpos (context->lexer); // save current lexer position.
    lexer_spos (context->lexer, 0); // rewind lexer to beginning.

    int label = 0; // start counting to the (future) binary position of lbl.
    do {
        lexer_nexttok (context->lexer, context->tok);
        
        if (context->tok->type == instruction) {
            label += INST_SIZE;
        }
        else if (context->tok->type == string) {
            label += strlen (context->tok->val) + 1;
        }
        else if (context->tok->type == labeldec) {
            if (strcmp (context->tok->val, lbl) == 0) {
                lexer_spos (context->lexer, realpos); // restore lexer position.
                return label;
            }
        }
    } while (context->tok->val != eof);

    lexer_spos (context->lexer, realpos); // restore lexer position.

    return -1;
}

static int binarylen (emitcontext_t * context) {
    int len = 0;
    do {
        lexer_nexttok (context->lexer, context->tok);

        if (context->tok->type == instruction) {
            len += INST_SIZE;
        }
        else if (context->tok->type == string) {
            len += strlen (context->tok->val) + 1;
        }
    } while (context->tok->value != eof);

    lexer_rewind (context->lexer);
    return len;
}
