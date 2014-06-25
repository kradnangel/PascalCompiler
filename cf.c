#include "common.h"

#include _YTAB_H_

static int folding_const = 0;

/* do constant folding.
 */
void const_folding(Node n)
{
    Symbol con;
    char little_buf[NAME_LEN];

    if ((generic(n->kids[0]->op) == CNST)
            && ((n->kids[1] == NULL) || (generic(n->kids[1]->op) == CNST)))
    {
        /* do not folding strings. */
        if ((n->kids[0]->syms[0]->type->type_id == TYPE_STRING) ||
                ((n->kids[1] != NULL) && (n->kids[1]->syms[0]->type->type_id == TYPE_STRING)))
            return;

        snprintf(little_buf, sizeof(little_buf) - 1, "g_con_%d", folding_const++);
        con = new_symbol(little_buf, DEF_CONST, n->kids[0]->syms[0]->type->type_id);
        n->syms[0] = con;

        switch(generic(n->op))
        {
            /*
            binary_xx(op,	realop)		singlar operator
            single_xx(op,  realop)		binary operator
            */
#define binary_xx(optype, realop) case optype: \
				switch(n->kids[0]->syms[0]->type->type_id) { \
						case TYPE_INTEGER: \
								con->v.i = (n->kids[0]->syms[0]->v.i) realop (n->kids[1]->syms[0]->v.i); \
								break; \
						case TYPE_BOOLEAN: \
								con->v.b = (n->kids[0]->syms[0]->v.b) realop (n->kids[1]->syms[0]->v.b); \
								break; \
						case TYPE_CHAR: \
								con->v.c = (n->kids[0]->syms[0]->v.c) realop (n->kids[1]->syms[0]->v.c); \
								break; \
						default: \
								break; \
				} \
				break;

#define real_binary_xx(optype, realop) case optype: \
				switch(n->kids[0]->syms[0]->type->type_id) { \
						case TYPE_INTEGER: \
								con->v.i = (n->kids[0]->syms[0]->v.i) realop (n->kids[1]->syms[0]->v.i); \
								break; \
						case TYPE_BOOLEAN: \
								con->v.b = (n->kids[0]->syms[0]->v.b) realop (n->kids[1]->syms[0]->v.b); \
								break; \
						case TYPE_REAL: \
								con->v.f = (n->kids[0]->syms[0]->v.f) realop (n->kids[1]->syms[0]->v.f); \
								break; \
						case TYPE_CHAR: \
								con->v.c = (n->kids[0]->syms[0]->v.c) realop (n->kids[1]->syms[0]->v.c); \
								break; \
						default: \
								break; \
				} \
				break;

#define single_xx(optype, realop) case optype: \
				switch(n->kids[0]->syms[0]->type->type_id) { \
						case TYPE_INTEGER: \
								con->v.i = realop (n->kids[0]->syms[0]->v.i); \
								break; \
						case TYPE_BOOLEAN: \
								con->v.b = realop (n->kids[0]->syms[0]->v.b); \
								break; \
						case TYPE_CHAR: \
								con->v.c = realop (n->kids[0]->syms[0]->v.c);\
								break; \
						default: \
								break; \
				} \
				break;

#define real_single_xx(optype, realop) case optype: \
				switch(n->kids[0]->syms[0]->type->type_id) { \
						case TYPE_INTEGER: \
								con->v.i = realop (n->kids[0]->syms[0]->v.i); \
								break; \
						case TYPE_BOOLEAN: \
								con->v.b = realop (n->kids[0]->syms[0]->v.b); \
								break; \
						case TYPE_REAL: \
								con->v.f = realop (n->kids[0]->syms[0]->v.f); \
								break; \
						case TYPE_CHAR: \
								con->v.c = realop (n->kids[0]->syms[0]->v.c);\
								break; \
						default: \
								break; \
				} \
				break;

            binary_xx(AND, &&);
            binary_xx(OR, ||);
            single_xx(NOT, !);
            single_xx(BCOM, !);
            real_single_xx(NEG, -);
            real_binary_xx(EQ, ==);
            real_binary_xx(NE, !=);
            real_binary_xx(GT, >);
            real_binary_xx(GE, >=);
            real_binary_xx(LT, <);
            real_binary_xx(LE, <=);
            binary_xx(BAND, &);
            binary_xx(BOR, |);
            real_binary_xx(ADD, +);
            real_binary_xx(SUB, -);
            real_binary_xx(DIV, /);
            real_binary_xx(MUL, *);
            binary_xx(MOD, %);
            binary_xx(RSH, >>);
            binary_xx(LSH, <<);
            single_xx(CVF, (float));
            single_xx(CVI, (int));
#undef single_xx
#undef real_single_xx
#undef binary_xx
#undef real_binary_xx
        default:
            assert(0);
            break;

        }
        
        n->kids[0] = n->kids[1] = NULL;
        n->op = CNST;

    }
}
