#ifndef _OPS_TPC_26609323423750235_H
#define _OPS_TPC_26609323423750235_H

/* system routine and functions. */
enum {
    SYS_ABS = 1,
    SYS_ODD,
    SYS_PRED,
    SYS_SQR,
    SYS_SQRT,
    SYS_SUCC,
    SYS_READ,
    SYS_READLN,
    SYS_WRITE,
    SYS_WRITELN
};

enum {
    /* node and tree operations . */
    CNST = 1 << 4,
    ARG = 2 << 4,
    ASGN = 3 << 4,
    INDIR = 4 << 4,
    SYS = 5 << 4,			/* for sys routine. */
    CVF = 7 << 4,
    CVI = 8 << 4,
    CVP = 9 << 4,
    CVB = 11 << 4,
    NEG = 12 << 4,
    CALL = 13 << 4,
    LOAD = 14 << 4,
    RET = 15 << 4,
    ADDRG = 16 << 4,
    ADDRF = 17 << 4,
    ADDRL = 18 << 4,
    ADD = 19 << 4,
    SUB = 20 << 4,
    LSH = 21 << 4,
    MOD = 22 << 4,
    RSH = 23 << 4,
    BAND = 24 << 4,
    BCOM = 25 << 4,
    BOR = 26 << 4,
    BXOR = 27 << 4,
    DIV = 28 << 4,
    MUL = 29 << 4,
    EQ = 30 << 4,
    GE = 31 << 4,
    GT = 32 << 4,
    LE = 33 << 4,
    LT = 34 << 4,
    NE = 35 << 4,
    JUMP = 36 << 4,
    LABEL = 37 << 4,
    AND = 38 << 4,
    NOT = 39 << 4,
    OR = 40 << 4,
    COND = 41 << 4,
    RIGHT = 42 << 4,
    FIELD = 43 << 4,
    INCR = 44 << 4,
    DECR = 45 << 4,
    ARRAY = 46 << 4,
    /* operations only appear in trees. */
    FUNCTION = 48 << 4,		/* function tree. */
    ROUTINE = 49 << 4,		/* routine tree. */
    HEADER = 50 << 4,		/* function header tree. */
    TAIL = 51 << 4,			/* function end tree. */
    BLOCKBEG = 52 << 4, 	/* block begin. */
    BLOCKEND = 53 << 4,		/* block end. */
    /* last */
    LASTOP
};

typedef struct _ops_entry
{
    char *name;
    int	op;
}
ops_entry;

char *get_op_name(int op);
int get_op(char *name);

#endif
