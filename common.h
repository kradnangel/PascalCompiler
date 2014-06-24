#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "error.h"

#define KEYWORD		-1

#define FILE_NAME_LEN	(64)
#define	_YTAB_H_	"rule.h"

#define DAG_DEBUG		(2)
#define CONST_FOLDING_DEBUG	(4)
#define COMMON_EXPR_DEBUG	(8)
#define SYSTEM_CALL_DEBUG	(16)
#define DEBUG			(0)


#define MAX_LINE_LENGTH		(20480)

#define STACK_SEG_SIZE		(2048)
#define TABLE_LEN			(128)
#define LABEL_LEN			(32)
#define LAST_ENTRY			(65536 * 1024)
#define NAME_LEN			(32)
#define SIZE_CHAR			(1)
#define SIZE_INTEGER			(4)
#define SIZE_REAL			(4)
#define SIZE_BOOLEAN			SIZE_INTEGER
#define SIZE_POINTER			(4)
#define S_STACK				SIZE_INTEGER
#define DEF_UNKNOWN			(0)
#define DEF_CONST			(1)
#define DEF_VAR				(2)
#define DEF_TYPE			(3)
#define DEF_FIELD			(4)
#define DEF_VALPARA			(5)
#define DEF_VARPARA			(6)
#define DEF_PROC			(7)
#define DEF_FUNCT			(8)
#define DEF_PROG			(9)
#define DEF_TEMPVAR			(10)
#define DEF_ELEMENT			(11)
#define DEF_LABEL			(12)

typedef int	boolean;

#define false 				0
#define true 				1
#define new_index(m)			++m##_index
#define size(x)				SIZE_##x

extern	FILE 	*codfp;
extern	FILE	*datfp;
extern  FILE 	*errfp;

typedef struct
{
    char name[NAME_LEN];
    int key;
    int attr;
    int ret_type;
    int arg_type;
}
KEYENTRY;

/* memory allocation arenas. */
enum { PERM=0, FUNC, STMT, TREE, DAG, LASTARENA };

/* position of file. */
typedef struct coord
{
    char *file;
    unsigned x, y;
}
Coordinate;

void *allocate(unsigned long n, unsigned a);

/* memory allocation */
#define NEW(p,a) ((p) = allocate(sizeof *(p), (a)))
#define NEW0(p,a) memset(NEW((p),(a)), 0, sizeof *(p))

#define NELEMS(a) ((int)(sizeof (a)/sizeof ((a)[0])))
#undef roundup
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))


/* types and storage class spcifiers */
enum {
    TYPE_UNKNOWN = 0,
    TYPE_INTEGER = 1,
    TYPE_CHAR = 2,
    TYPE_REAL = 3,
    TYPE_BOOLEAN = 4,
    TYPE_ARRAY = 5,
    TYPE_ENUM = 6,
    TYPE_SUBRANGE = 7,
    TYPE_RECORD = 8,
    TYPE_VOID = 9,
    TYPE_STRING = 10,
    TYPE_POINTER = 11,
    TYPE_DOUBLE = 12,
    TYPE_FUNCTION = 13,
    TYPE_LONG = 14,
    TYPE_CONST = 15
};

enum {
    F=TYPE_REAL,
    I=TYPE_INTEGER,
    P=TYPE_POINTER,
    V=TYPE_VOID,
    B=TYPE_RECORD
};

/** for operations. **/
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
    TAIL = 51 << 4,		/* function end tree. */
    BLOCKBEG = 52 << 4, 	/* block begin. */
    BLOCKEND = 53 << 4,		/* block end. */
    /* last */
    LASTOP
};

#define generic(op)  ((op)&0x3F0)
#define opindex(op) (((op)>>4)&0x3F)

/* symbol and type definition. */
#include "symtab.h"

typedef struct list *List;
struct list
{
    void *x;					/* data or tail of list. */
    List link;					/* next in list. */
};

List list_append(List list, void *x);
int list_length(List list);
void **list_ltov(List list, unsigned arena);
void list_clear(List list);


typedef struct
{
    union {
        struct
        {
            int argc;
            char **argv;
            Symtab tab;
        }
        program;

        struct
        {
            Symtab tab;
        }
        main;
    }u;
}
Env;

extern Env global_env;
extern Env main_env;

/* trees */
#include "tree.h"

/* nodes and dags. */
#include "dag.h"

typedef struct metrics
{
    unsigned char size, align, outofline;
}
Metrics;

typedef struct blockcontext
{
    unsigned char size;
}
BlockContext;

typedef struct interface
    {

        /* type interface */
        Metrics charmetric;
        Metrics shortmetric;
        Metrics intmetric;
        Metrics floatmetric;
        Metrics doublemetric;
        Metrics pointermetric;
        Metrics structmetric;

        /* function interface. */
        int (*program_begin)(Env *);
        int (*program_end)(Env *);
        int (*main_begin)(Env *);
        int (*main_end)(Env *);
        int (*def_address)(Symbol p, Symbol q, long n);
        int (*block_begin)(BlockContext *context);
        int (*block_end)(BlockContext *context);
        int (*global_variable)(Symbol symbol);
        int (*local_variable)(Symbol symbol);
        int (*def_label)(Symbol symbol);
        int (*def_const)(int type, Value value);
        int (*alloc_space)(int n);
        int (*mark_node)(Node rootnode);
        int (*gen_code)(Node rootnode);
        int (*function_process)(List dags);
        void (*def_export)(Symbol);
        void (*def_import)(Symbol);
        /* Xinterface x; */
    }
Interface;

extern Interface *IR;

int get_keytable_size();
void *allocate(unsigned long n, unsigned a);
void deallocate(unsigned a);
void *newarray(unsigned long m, unsigned long n, unsigned a);
void const_folding(Node n);

#endif

