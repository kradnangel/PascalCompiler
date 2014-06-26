#define  X86_LINUX
#include  <stdio.h>
#include  "common.h"
#include  "symtab.h"
#include  "error.h"
#include <time.h>
#include  _YTAB_H_
#define O fprintf(codfp,
#define P fprintf(datfp,
#define TYPEERR parse_error("Type Error.","");
#define LABEL_SLINK "static_link"
#define LABEL_RETVAL    "retval_addr"
#define LABEL_HIRETVAL  "retval_addr_hi"
#define STACK_SEG       512
#define  MAX_LOOP  16
#define  MAX_CALL 16
#define  STMT_STACK 64

void set_call_stack_top(symbol *);
void push_call_stack(symtab *);
symtab *top_call_stack();
symtab *pop_call_stack();
symbol *top_call_symbol();

static symtab *rtn =NULL;
static symtab *ret =NULL;
static symbol *arg = NULL;
static Symbol p;
static Symtab ptab;
static int gen_level = 0;
static int gen_address = 0;
static int jump_index = 0;
extern char datname[FILE_NAME_LEN];
static void gen_abs(int arg_type);
static void gen_odd(int arg_type);
static void gen_sqr(int arg_type);
static void gen_sqrt(int arg_type);
static void gen_read1(int arg_type);
static void gen_write1(int arg_type);;
static void gen_writeln(int arg_type);
static void gen_program_head();
static void gen_program_prologue(symtab *);
static void gen_program_epilogue(symtab *);
static void gen_main_prologue(symtab *);
static void gen_main_epilogue(symtab *);
static void gen_routine_prologue(symtab *);
static void gen_routine_epilogue(symtab *);
static void gen_local_args(symtab *);
static void gen_dos_push_op(Type ptype);
static void gen_load_value(symbol *);
static void gen_function_assign(symtab *,int);
static void gen_procedure_call(symtab *);
static void gen_assign(symbol *, int);
static void gen_expression(Type, int);
static void gen_negate(symbol *);
static void gen_expr(Type, int);
static void gen_term(Type, int);
static void gen_factor(symbol *);
static void gen_array_factor(symbol *);
static void gen_record_factor(symbol *, symbol *);
static void gen_first_arg(int);
static int blockbegin(BlockContext *context){return 0;}
static int blockend(BlockContext *context){return 0;}
static int globalvariable(Symbol symbol){return 0;}
static int localvariable(Symbol symbol){return 0;}
static int deflabel(Symbol symbol){return 0;}
static int defaddress(Symbol p, Symbol q, long n){return 0;}
static int defconst(int type, Value value){return 0;}
static int allocspace(int n){return n;}
static int marknode(Node rootnode){return 0;}
static void defexport(Symbol sym){}
static void defimport(Symbol sym){}

#ifdef LABEL_RETVAL
#undef LABEL_RETVAL
#define LABEL_RETVAL "-4(%ebp)"
#endif

#ifdef LABEL_SLINK
#undef LABEL_SLINK
#define LABEL_SLINK	"8(%ebp)"
#endif

static int calltime = MAX_CALL - 1;
static symbol *call_sym[MAX_CALL];
static symtab *call_stk[MAX_CALL];

void push_call_stack(symtab *sym){
    call_stk[calltime] = sym;
    call_sym[calltime] = sym->args;
    ret = sym;
    calltime--;
    if (calltime == -1)
        internal_error("Stack Overflow.");
}

symtab *pop_call_stack(){
    calltime++;
    if (calltime == MAX_CALL)
        internal_error("Stack Underflow.");
    ret = call_stk[calltime];
    arg = call_sym[calltime];
    return call_stk[calltime];
}

symtab *top_call_stack(){
    return call_stk[calltime + 1];
}

void set_call_stack_top(symbol *p){
    call_sym[calltime + 1] = p;
}

symbol *top_call_symbol(){
    return call_sym[calltime + 1];
}

static void gen_sys_routine(int routine_id, int arg_type){
	switch(routine_id){
		case fABS:
			gen_abs(arg_type);
			break;
		case fODD:
			gen_odd(arg_type);
			break; 
		case fSQR:
			gen_sqr(arg_type);
			break;
		case fSQRT:
			gen_sqrt(arg_type);
			break;
		case fSUCC:
			O "  incl %%eax\n");
			break;
		case  fPRED:
			O "  decl %%eax\n");
			break;
		case pREAD:
			gen_read1(arg_type);
			break;
		case pWRITE:
			gen_write1(arg_type);
			break;
		case pREADLN:
			gen_read1(arg_type);
			break;
		case  pWRITELN:
			gen_writeln(arg_type);
			break;
		default:
			break;
	}
}
static void gen_abs(int arg_type){
	O "  pushl %%ebp\n");
	switch(arg_type){
		case TYPE_BOOLEAN:
		case TYPE_INTEGER:
			O "  call _abs_int\n");
			break;
		case TYPE_CHAR:
			O "  xorb %%ah,%%ah\n");
			O "  call _abs_int\n");
			break;
		default:
			break;
	}
	O "  addl $8, %%esp\n");
}

static void gen_odd(int arg_type){
	O "  pushl %%bp\n");
	switch(arg_type){
		case TYPE_BOOLEAN:
		case TYPE_INTEGER:
			O "  call _odd_int\n");
			break;
		case TYPE_CHAR:
			O "  xorb %%ah,%%ah\n");
			O "  call _odd_int\n");
			break;
		default:
			break;
	}
	O "  addl $8, %%esp\n");
}

static void gen_sqr(int arg_type){
	O "  pushl %%bp\n");
	switch(arg_type){
		case TYPE_BOOLEAN:
		case TYPE_INTEGER:
			O "  call _sqr_int\n");
			break;
		case TYPE_CHAR:
			O "  xorb %%ah,%%ah\n");
			O "  call _sqr_int\n");
			break;
		default:
			break;
	}
	O "  addl $8, %%esp\n");
}

static void gen_sqrt(int arg_type){
	O "  pushl %%ebp\n");
	switch(arg_type){
		case TYPE_BOOLEAN:
		case TYPE_INTEGER:
			O "  call _sqrt_int\n");
			break;
		case TYPE_CHAR:
			O "  xorb %%ah,%%ah\n");
			O "  call _sqrt_int\n");
			break;
		default:
			break;
		}
		O "  addl $8, %%esp\n");
}

static void gen_read1(int arg_type){
	O "  pushl %%eax\n");
	O "  pushl %%ebp\n");
	switch(arg_type){
		case  TYPE_INTEGER:
			O "  call _read_int\n");
			break;
		case  TYPE_REAL:
			break;
		case TYPE_CHAR:
			O "  call _read_char\n");
			break;
		case TYPE_STRING:
			O "  call _read_string\n");
			break;
		case   TYPE_BOOLEAN:
		default:
			parse_error("Type Error.", "");
			break;
	}
	O "  addl $8, %%esp\n");
}

static void gen_write1(int arg_type){
	switch(arg_type){
		case TYPE_BOOLEAN:
		case TYPE_INTEGER:
			O "  pushl %%eax\n");
			O "  pushl %%ebp\n");
			O "  call _write_int\n");
			break;
		case TYPE_CHAR:
			O "  pushl %%eax\n");
			O "  pushl %%ebp\n");
			O "  call _write_char\n");
			break;
		case TYPE_STRING:
			O "  pushl %%eax\n");
			O "  pushl %%ebp\n");
			O "  call _write_string\n");
			break;
		default:
			break;
	}
	O "  addl $8, %%esp\n");
}

static void gen_writeln(int arg_type){
	switch(arg_type){
		case TYPE_BOOLEAN:
		case TYPE_INTEGER:
			O "  pushl %%eax\n");
			O "  pushl %%ebp\n");
			O "  call _writeln_int\n");
			break;
		case TYPE_CHAR:
			O "  pushl %%eax\n");
			O "  pushl %%ebp\n");
			O "  call _writeln_char\n");
			break;
		case  TYPE_STRING:
			O "  pushl %%eax\n");
			O "  pushl %%ebp\n");
			O "  call _writeln_string\n");
			break;
		default:
			break;
	}
	O "  addl $8, %%esp\n");
}

static void gen_program_prologue(symtab *ptab){
	gen_program_head();
}

static void gen_program_head(){
	P ".file \"%s\"\n\n", datname);
	P "sys_call_id = 0x80\n");
	P "exit_syscall = 0x1\n\n");
}

static void gen_program_epilogue(symtab *ptab){
	symbol *p;
	O "\n\n.globl _start\n");
	O "_start:\n");
	O "  call _main\n");
	O "  movl $0, %%ebx\n");
	O "  movl $exit_syscall, %%eax\n");
	O "  int  $sys_call_id\n");
	for(p = ptab->locals; p; p = p->next){
		if (p->defn != DEF_CONST){
				switch (p->type->type_id){
					case TYPE_BOOLEAN:
					case TYPE_INTEGER:
						O "  .comm %s,%d,%d\n", p->rname, IR->intmetric.size, IR->intmetric.align);
						break;
					case TYPE_CHAR:
						O "  .comm %s,%d,%d\n", p->rname, IR->charmetric.size, IR->intmetric.align);
					   break;
					case TYPE_ARRAY:
						if(p->type_link->last->type->type_id == TYPE_INTEGER
						   || p->type_link->last->type->type_id == TYPE_BOOLEAN)
							O"  .comm %s, %d, %d\n" , p->rname, 
							p->type_link->num_ele * IR->intmetric.size, IR->intmetric.align);
						else if(p->type_link->last->type->type_id == TYPE_CHAR)
							O"  .comm %s, %d, %d\n", p->rname, p->type_link->num_ele, IR->intmetric.align);
						else
							parse_error("Array Element Unsupported.","");
						if(p->defn != DEF_CONST)
							continue;
						break;
					case   TYPE_RECORD:
						O"  .comm %s, %d, %d\n", p->rname, p->type_link->size, IR->intmetric.align);
						continue;
					default:
						break;
				}
		}
	}
	P ".data\n");
	for(p = ptab->locals; p; p = p->next){
		if (p->defn == DEF_CONST){
				switch (p->type->type_id){
					case TYPE_BOOLEAN:
					case TYPE_INTEGER:
						P ".globl %s\n", p->rname);
						P "  .align %d\n", IR->intmetric.align);
						P "  .type %s @object\n", p->rname);
						P "  .size %s, %d\n", p->rname, IR->intmetric.size);
						P "%s:\n", p->rname);
						P "  .long %d\n", p->v.i);
						break;
                    case TYPE_CHAR:
                        P ".globl %s\n", p->rname);
                        P "  .type %s @object\n", p->rname);
                        P "  .size %s, %d\n", p->rname, IR->charmetric.size);
                        P "%s:\n", p->rname);
                        P "  .byte %d\n", p->v.c);
                        break;
					case TYPE_STRING:
						P ".globl %s\n", p->rname);
						P "  .section .rodata\n");
						P "  .align %d\n", IR->pointermetric.align);
						P ".LC%s:\n", p->rname);
						if ((p->v.s[0] == '\'') && (p->v.s[strlen(p->v.s) - 1] == '\'')){
							p->v.s[strlen(p->v.s) - 1] = '\0';
							P "  .string \"%s\"\n", p->v.s + 1);
						}else
							P "  .string \"%s\"\n", p->v.s);
						P "  .data\n");
						P "  .align %d\n", IR->pointermetric.align);
						P "  .type %s @object\n", p->rname);
						P "  .size %s, %d\n", p->rname, IR->pointermetric.size);
						P "%s:\n", p->rname);
						P "  .long .LC%s\n", p->rname);
						break;
					default:
						break;
				}
		}
	}
}

static void gen_main_prologue(symtab *ptab){
	O "  .text\n");
	O ".globl _main\n");
	O "  .type _main, @function\n");
	O "_main:\n");
	O "  pushl %%ebp\n");
	O "  movl %%esp, %%ebp\n");
}

static void gen_main_epilogue(symtab *ptab){
	O"  leave\n");
	O"  ret\n");
}

static void gen_routine_prologue(symtab *ptab){
	if(ptab->defn == DEF_PROG)
		return;
	gen_local_args(ptab);
	O "  .text\n");
	O ".globl %s\n", ptab->rname);
	O "  .type %s, @function\n", ptab->rname);
	O "%s:\n", ptab->rname);
	O "  pushl %%ebp\n");
	O "  movl %%esp, %%ebp\n");
	if(ptab->defn == DEF_FUNCT)
		O"  subl $8, %%esp\n");
	O "  subl $%d, %%esp\n", ptab->local_size);
}

static void gen_local_args(symtab *ptab){
	symbol *p;
	char tp[10];
	for(p = ptab->locals; p->next; p = p->next){
		switch(p->type->type_id){
    		case TYPE_INTEGER:
    		case TYPE_BOOLEAN:
    			sprintf(tp, "word ptr");
    			break;
    		case TYPE_REAL:
    			sprintf(tp, "dword ptr");
    			break;
            case TYPE_CHAR:
                sprintf(tp, "byte ptr");
                break;
    		case TYPE_ARRAY:
    			if(p->type_link->last->type->type_id ==	TYPE_INTEGER
    			   || p->type_link->last->type->type_id == TYPE_BOOLEAN)
    				sprintf(tp, "word ptr");
    			else if (p->type_link->last->type->type_id == TYPE_CHAR)
    				sprintf(tp, "byte ptr");
    			break;
    		case TYPE_RECORD:
    			sprintf(tp, "byte ptr");
    			break;
    		default:
    			break;
		}
		snprintf(p->rname, sizeof(p->rname) - 1, "-%d(%%ebp)", p->offset);
	}
	for(p = ptab->args;p ;p = p->next){
		switch(p->type->type_id){
    		case TYPE_INTEGER:
    		case TYPE_BOOLEAN:
    			sprintf(tp, "word  ptr");
    			break;
            case TYPE_CHAR:
                sprintf(tp, "byte  ptr");
                break;
    		case TYPE_REAL:
    			sprintf(tp, "dword  ptr");
    			break;
    		default:
    			break;
		}
		snprintf(p->rname,sizeof(p->rname) - 1,"%d(%%ebp)", p->offset);
	}
}

static void gen_routine_epilogue(symtab *ptab){
	if(ptab->defn == DEF_PROG)
		return;

	if(ptab->defn == DEF_FUNCT){
		switch(ptab->type->type_id){
		case TYPE_INTEGER:
		case TYPE_BOOLEAN:
			O "\n  movl %s, %%eax\n", LABEL_RETVAL); 
			break;
		case TYPE_CHAR:
			O "\n  xorb %%ah, %%ah\n");
			O "\n  movl %s, %%eax\n", LABEL_RETVAL); 
			break;
		case TYPE_REAL:
			break;
		}
	}
	O "  leave\n");
	O "  ret\n");
	O "  .size %s, .-%s\n",	ptab->rname, ptab->rname);
}

static void gen_dos_push_op(Type ptype){
	switch(ptype->type_id){
    	case  TYPE_BOOLEAN:
    	case  TYPE_INTEGER:
        case  TYPE_CHAR:
    		O"  pushl %%eax\n");
    		break;
    	case  TYPE_REAL:
    		O"  pushl %%edx\n");
    		O"  pushl %%eax\n");
    		break;
	}
}

static void gen_load_value(symbol *p){
	if(p->defn == DEF_VARPARA){
		O"  movl 4(%%ebp), %%ebx\n");
		switch(p->type->type_id){
    		case  TYPE_INTEGER:
            case  TYPE_BOOLEAN:
                O"  movl (%%ebx), %%eax\n");
                break;
            case  TYPE_CHAR:
    			O"  xorl %%eax, %%eax\n");
    			O"  movb (%%ebx), %%al\n");
    			break;
    		case TYPE_REAL:
    			O"  movl (%%ebx), %%eax\n");
    			O "  movl 4(%%ebx), %%edx\n");
    			break;
		}
	}
	else if (p->tab->level == 0 || p->tab == top_symtab_stack()){
		switch(p->type->type_id){
    		case TYPE_CHAR:
    			O"  subl %%eax, %%eax\n");
    			O"\b  movb %s, %%al\n", p->rname);
    			break;
    		case TYPE_INTEGER:
    		case TYPE_BOOLEAN:
    			O"   movl %s, %%eax\n",p->rname);
    			break;
    		case TYPE_ARRAY:
    			O"  popl %%ebx\n");
    			if(p->type_link->last->type->type_id == TYPE_INTEGER ||
                   p->type_link->last->type->type_id == TYPE_BOOLEAN)
    				O "  movl (%%ebx), %%eax\n");
    			else if (p->type_link->last->type->type_id == TYPE_CHAR)
    				O "  movb (%%ebx), %%al\n");
    			break;
    		default:
    			break;
    		}
	}
}

static void gen_load_address(symbol *p){
	symtab *ptab;
	int  i, n;
	switch(p->defn){
	case DEF_VARPARA:
		O "  movl %s, %%eax\n", p->rname);
		break;
	case DEF_VAR:
		if(p->tab->level == 0 || p->tab == top_symtab_stack()){
			O "  leal %s, %%eax\n", p->rname);
		}
		else{
			ptab = top_symtab_stack();
			n = p->tab->level - ptab->level + 1;
			O"  movl %%ebp, %%ebx\n");
			for (i = 0; i < n; i++)
				O "  movl %s, %%ebp\n", LABEL_SLINK);
			O "  leal %s, %%eax\n",
					p->rname);
			O"  movl %%ebx, %%ebp\n");
		}
		break;
	case DEF_VALPARA:
		O "  leal %s, %%eax\n", p->rname);
		break;
	default:
		break;
	}
}

static void gen_load_field(symbol*p){
	if(!p)
		return;
	O "  popl %%ebx\n");
	switch(p->type->type_id){
    	case TYPE_INTEGER:
    	case TYPE_BOOLEAN:
    		O"  movl (%%ebx), %%eax\n");
    		break;
    	case TYPE_CHAR:
    		O"  movb (%%ebx), %%al\n");
    		break;
    	default:
    		break;
	}
}

static void gen_function_assign(symtab *ptab, int srctype){
	if(!ptab)
		return;

	if(ptab->type->type_id != srctype){
		TYPEERR
		return;
	}
	switch(ptab->type->type_id){
    	case  TYPE_BOOLEAN:
    	case  TYPE_INTEGER:
    		O "  movl %%eax, %s\n", LABEL_RETVAL);
    		break;
    	case  TYPE_CHAR:
            O "  xorb %%ah, %%ah\n");
            O "  movl %%eax, %s\n", LABEL_RETVAL);
            break;
    	default:
    		break;
	}
}

static void gen_procedure_call(symtab *ptab){
	symtab *caller = top_symtab_stack();
	symtab *callee = ptab;
	int i, n;
	if(!caller || !callee){
		parse_error("Procedure Undefine.","");
		return;
	}
	n = (callee->level) - (caller->level) + 1;
	if(callee->level == caller->level + 1){
		O"  pushl %%ebp\n");
	}else if (callee->level == caller->level){
		O "  pushl %s\n", LABEL_SLINK);
	}
	else if(callee->level < caller->level){
		O "  movl %%ebp, %%ebx\n");
		for(i = 0; i < n; i++)
			O"  movl %s, %%ebp\n", LABEL_SLINK);
		O "  pushl %%ebp\n");
		O "  movl %%ebx, %%ebp\n");
	}else
		return;
	O "  call %s\n", ptab->rname);
	O "  addl $%d, %%esp\n", ptab->args_size + IR->intmetric.size);
}

static void gen_first_arg(int ptype){
	rtn = top_call_stack();
	if(rtn)
		arg = rtn->args;
	else
		return;
	if(!arg)
		return;
	switch(arg->type->type_id){
    	case  TYPE_CHAR:
            O"  xorb %%ah, %%ah\n");
        case  TYPE_REAL:
            break;
        case  TYPE_INTEGER:
        case  TYPE_BOOLEAN:
        default:
    		O"  pushl %%eax\n");
    		break;
	}
}

static void gen_args(int ptype){
	arg = top_call_symbol();
	if(arg->next)
		arg = arg->next;
	else
		return;
	set_call_stack_top(arg);
	switch(arg->type->type_id){
    	case TYPE_CHAR:
            O"  xorb %%ah, %%ah\n");
        case  TYPE_REAL:
    		break;
    	case  TYPE_INTEGER:
    	case  TYPE_BOOLEAN:
    	default:
    		O"  pushl %%eax\n");
    		break;
	}
}

static void gen_assign(symbol *p, int srctype){
	symtab *ptab;
	int  i, n;
	if (!p)
		return;
	if ((p->type->type_id != TYPE_ARRAY) && (p->type->type_id != TYPE_RECORD) && 
        (p->type->type_id != srctype)){
		TYPEERR
		return;
	}
	if((p->type->type_id == TYPE_ARRAY) 
		&& (srctype != p->type->first->type->type_id)){
		TYPEERR
		return;
	}
	if((p->type->type_id == TYPE_RECORD)) {
		TYPEERR
		return;
	}
	switch(p->defn){
    	case DEF_VARPARA:
    		O "  pushl %%eax\n");
    		O "  movl %s, %%eax\n",	p->rname);
    		break;
    	case DEF_FIELD:
    		O "  popl %%ebx\n");
    		if(p->type->type_id == TYPE_INTEGER ||p->type->type_id == TYPE_BOOLEAN)
    			O "  movl %%eax, (%%ebx)\n");
    		else if (p->type->type_id == TYPE_CHAR)
    			O "  movb %%al, (%%ebx)\n");
    		return;
    	case DEF_VAR:
    	case DEF_CONST:
    	case DEF_ELEMENT:
    		if(p->type->type_id == TYPE_ARRAY){
    			O"  pushl %%eax\n");
    			break;
    		}
    		if(p->tab->level == 0 ||p->type->type_id == TYPE_REAL)
    			break;
    		else if( p->tab->level
    				 && p->tab == top_symtab_stack()){
    			O "  pushl %%eax\n");
    			O "  leal %s, %%eax\n", p->rname);
    			O "  pushl %%eax\n");
    			break;
    		} else {
    			ptab = top_symtab_stack();
    			n = ptab->level - p->tab->level;
    			O "  pushl %%eax\n");
    			O "  movl %%ebp, %%ebx\n");
    			for(i =0;i<n;i++)
    				O"  movl %s,%%ebp\n", LABEL_SLINK);
    			O "  leal %s, %%eax\n", p->rname);
    			O "  movl %%ebx, %%ebp\n");
    			O "  pushl %%eax\n");
    		}
    		break;
    	case DEF_VALPARA:
    		if(p->tab->level==0 || p->tab==top_symtab_stack())
    			O"  pushl %%eax\n");
    		O "  leal %s, %%eax\n", p->rname);
    		O " \rpushl %%eax\n");
    		break;
    	default:
    		parse_error("Need lvalue.","");
    		break;
	}

	switch(p->type->type_id){
    	case TYPE_INTEGER:
    	case TYPE_BOOLEAN:
    		if (p->tab->level){
    			O "  popl %%ebx\n");
    			O "  popl %%eax\n");
    			O "  movl %%eax, (%%ebx)\n");
    		}else
    			O "  movl %%eax, %s\n", p->rname);
    		break;
        case TYPE_CHAR:
            if(p->tab->level){
                O "  popl %%bx\n");
                O "  popl %%ax\n");
                O " movb %%al, (%%bx)\n");
            }else
                O"  movb %%al,%s\n", p->rname);
            break;
    	case TYPE_STRING:
    		O "  movl $%d, %%cx\n", strlen(p->v.s));
    		O "  popl esi\n");
    		O "  popl edi\n");
    		O "  movl %%ds, %%eax\n");
    		O "  movl %%eax, %%es\n");
    		O "  cld\n");
    		O "  rep movsb\n");
    		break;
    	case TYPE_REAL:
    		break;
    	case TYPE_ARRAY:
    		if (p->type_link->last->type->type_id == TYPE_INTEGER ||
                p->type_link->last->type->type_id == TYPE_BOOLEAN ){
    			O "  popl %%eax\n");
    			O "  popl %%ebx\n");
    			O "  movl %%eax, (%%ebx)\n");

    		}else if (p->type_link->last->type->type_id == TYPE_CHAR){
    			O "  popl %%eax\n");
    			O "  popl %%ebx\n");
    			O "  movb %%al, (%%ebx)\n");
    		}
    		break;
    	default:
    		break;
	}
}

static void gen_cond_jump(int true_or_false, Symbol label){
	O "  cmpl $1, %%eax\n");
	if (true_or_false)
		O "  jge %s\n", label->name);
	else
		O "  jl %s\n", label->name);
}

static void gen_jump(Symbol label){
	O "  jmp %s\n", label->name);
}

static void gen_label(Symbol label){
	O "%s:\n", label->name);
}

static void gen_incr(Symbol sym){
	switch (sym->type->type_id){
    	case TYPE_BOOLEAN:
    	case TYPE_INTEGER:
    		O "  incl %s\n", sym->rname);
    		break;
    	case TYPE_CHAR:
    		O "  incb %s\n", sym->rname);
    		break;
    	default:
    		TYPEERR
    		break;
	}
}

static void gen_decr(Symbol sym){
	switch (sym->type->type_id){
    	case TYPE_BOOLEAN:
    	case TYPE_INTEGER:
    		O "decl %s\n", sym->rname);
    		break;
    	case TYPE_CHAR:
    		O "decb %s\n", sym->rname);
    		break;
    	default:
    		TYPEERR
    		break;
	}
}

static void gen_expression(Type type, int op){
	if (type == NULL){
		return;
	}
	if (type->type_id == TYPE_INTEGER || type->type_id == TYPE_BOOLEAN){
		O "  popl %%edx\n");
		O "  cmpl %%eax, %%edx\n");
	}else if(type->type_id == TYPE_CHAR){
		O "  popl %%edx\n");
		O "  cmpb %%al, %%dl\n");
	}else if (type->type_id == TYPE_STRING){
		O"  popl %%edi\n");
		O "  popl %%esi\n");
		O "  movl %%ds, %%eax\n");
		O "  movl %%eax, %%es\n");
		O "  cld\n");
		O "  movl $%d, %%ecx\n",strlen(p->v.s));
		O "  repe  cmpsb\n");
	}else{
		TYPEERR
		return;
	}
	O "  movl $1, %%eax\n");
	switch(generic(op)){
    	case GE:
    		O "  jge j_%03d\n", new_index(jump));
    		break;
    	case LE:
    		O "  jle j_%03d\n", new_index(jump));
    		break;
    	case GT:
    		O "  jg j_%03d\n", new_index(jump));
    		break;
    	case LT:
    		O "  jl j_%03d\n", new_index(jump));
    		break;
    	case EQ:
    		O "  je j_%03d\n", new_index(jump));
    		break;
    	case NE:
    		O "  jne j_%03d\n", new_index(jump));
    		break;
	}
	O "  xorl %%eax, %%eax\n");
	O "j_%03d:\n", jump_index);
}

static void gen_negate(symbol *p){
	if (!p)
		return;
	if (p->defn != DEF_VAR && p->defn != DEF_VALPARA && 
        p->defn != DEF_VARPARA && p->defn != DEF_CONST){
		parse_error("Need Variable.", "");
		return;
	}
	switch(p->type->type_id){
    	case TYPE_INTEGER:
    		O "  negl %%eax\n");
    		break;
    	default:
    		TYPEERR
    		break;
	}
}

static void gen_expr(Type type, int op){
	if (type == NULL){
		return;
	}
	switch(generic(op)){
    	case ADD:
    		if (type->type_id == TYPE_INTEGER){
    			O"  popl %%edx\n");
    			O "  addl %%edx, %%eax\n");
    		}else if (type->type_id == TYPE_REAL){} else
    			TYPEERR
    		break;
    	case SUB:
    		if (type->type_id==TYPE_INTEGER){
    			O "  popl %%edx\n");
    			O "  subl %%eax, %%edx\n");
    			O "  movl %%edx, %%eax\n");
    		}else if  (type->type_id == TYPE_REAL){} else
    			TYPEERR
    		break;
    	case OR:
    		if (type->type_id == TYPE_BOOLEAN){
    			O "  popl %%edx\n");
    			O "  orl %%edx, %%eax\n");
    		}
    		else
    			TYPEERR
    		break;
    	default:
    		break;
	}
}

static void gen_term(Type type, int op){
	if (type == NULL)
		return;
	switch(generic(op)){
    	case MUL:
    		if (type->type_id == TYPE_INTEGER){
    			O "  popl %%edx\n");
    			O "  imul %%edx\n");
    		}else if (type->type_id == TYPE_REAL){}else
    			TYPEERR
    		break;
    	case DIV:
    		if (type->type_id == TYPE_INTEGER){
    			O"  movl %%eax, %%ecx\n");
    			O "  popl %%eax\n");
    			O "  subl %%edx,%%edx\n");
    			O "  idiv %%ecx\n");
    		}else
    			TYPEERR
    		break;
    	case MOD:
    		if (type->type_id == TYPE_INTEGER){
    			O "  movl %%eax, %%ecx\n");
    			O "  popl %%eax\n");
    			O "  subl %%edx,%%edx\n");
    			O "  idiv %%ecx\n");
    			O "  movl %%edx, %%eax\n");
    		}else
    			TYPEERR
    		break;
    	case AND:
    		if (type->type_id != TYPE_BOOLEAN)
    			parse_error("boolean type expected.","");
    		else{
    			O "  popl %%edx\n");
    			O "  andl %%edx, %%eax\n");
    		}
    		break;
    	default:
    		break;
	}
}

static void gen_factor(symbol *p){
	symtab *ptab;
	int i, n;
	if (!p)
		return;
	if (p->type->type_id == TYPE_ARRAY){
		parse_error("Need Array Element.","");
		return;
	}
	if (p->defn == DEF_CONST || p->defn == DEF_ELEMENT){
		switch(p->type->type_id){
    		case TYPE_BOOLEAN:
    			O "  movl $%d, %%eax\n",p->v.b);
    			break;
    		case TYPE_INTEGER:
    			if (p->defn == DEF_ELEMENT){
    				O "  movl $%d, %%eax\n",
    						p->v.i);
    			}else{
    				short tt = (short)p->v.i;
    				O "  movl $%d, %%eax\n", tt);
    			}
    			break;
    		case TYPE_CHAR:
    			O "  movb $%d,%%al\n",p->v.c);
    			break;
    		case TYPE_STRING:
    			O "  movl %s, %%eax\n",p->rname);
    			break;
    		default:
    			break;
		}
	}
	else if (p->defn == DEF_VARPARA){
		O "  movl %s, %%ebx\n", p->rname);
		O "  movl (%%ebx), %%eax\n");
	}
	else if (p->defn == DEF_VAR ||p->defn == DEF_VALPARA){
		if (p->tab == top_symtab_stack() || p->tab->level == 0){
			switch(p->type->type_id){
    			case TYPE_BOOLEAN:
    			case TYPE_INTEGER:
    				O "  movl %s, %%eax\n",p->rname);
    				break;
    			case TYPE_CHAR:
                    O "  subl %%eax,%%eax\n");
                    O "  movb %s, %%al\n",p->rname);
                    break;
                case TYPE_REAL:
    				break;
			}
		}
		if (p->defn == DEF_VAR){
			ptab = top_symtab_stack();
			n = ptab->level - p->tab->level;
			if (n <= 0)
				return;
			O "  movl %%ebx,%%ebp\n");
			for(i = 0; i<n; i++)
				O "  movl %s, %%ebp\n",	LABEL_SLINK);
			switch(p->type->type_id){			
    			case TYPE_BOOLEAN:
                case TYPE_INTEGER:
    				O "  movl %s, %%eax\n", p->rname);
    				break;
    			case TYPE_CHAR:
    				O "  movb %s, %%al\n", p->rname);
    				break;
    			default:
    				break;
			}
			O "  movl %%ebx, %%ebp\n");
		}
	}
}

static void gen_not_factor(symbol *p){
	if (!p)
		return;
	if (p->type->type_id!= TYPE_BOOLEAN)
		TYPEERR
	gen_factor(p);
	O "  andl $1, %%eax\n");
	O "  xorl $1, %%eax\n");
}

static void gen_array_factor(symbol *p){
	if (p->type_link->first->v.i >= 0)
		O "  subl $%d, %%eax\n", p->type_link->first->v.i);
	else
		O "  subl $-%d, %%eax\n", -(p->type_link->first->v.i));
	O "  movl $%d, %%ecx\n", get_symbol_size(p->type_link->last));
	O "  imul %%ecx\n");
	O "  popl %%edx\n");
	if (p->tab->level)
		O "  subl %%eax, %%edx\n");
	else
		O "  addl %%eax, %%edx\n");
	O "  pushl %%edx\n");
}

static void gen_record_factor(symbol *var, symbol *p){
	if (!p || !var || p->defn != DEF_FIELD)
		return;
	O "  popl %%eax\n");
	O "  movl $%d, %%edx\n",p->offset);
	if (var->tab->level)
		O "  subl %%edx, %%eax\n");
	else
		O "  addl %%edx, %%eax\n");
	O "  pushl %%eax\n");
}

static int programbegin(Env *global){
	gen_program_prologue(global->u.program.tab);
	return 0;
}

static int programend(Env *global){
	gen_program_epilogue(global->u.program.tab);
	return 0;
}

static int mainbegin(Env *main){
	gen_main_prologue(main->u.main.tab);
	return 0;
}

static int mainend(Env *main){
	gen_main_epilogue(main->u.main.tab);
	return 0;
}

static int gen_linux_code(Node rootnode){
	int ret = 0;
	Node pnode;
	gen_level++;
	switch (generic(rootnode->op)){
    	case ARG:
    		if (!rootnode->kids[0]){
    			return ERROR_SUCCESS;
    		}
    		gen_linux_code(rootnode->kids[0]);
    		gen_first_arg(rootnode->kids[0]->type->type_id);
    		pnode = rootnode->kids[1];
    		while(pnode){
    			if (!pnode->kids[0])
    				break;
    			gen_linux_code(pnode->kids[0]);
    			gen_args(pnode->kids[0]->type->type_id);
    			pnode = pnode->kids[1];
    		}
    		gen_level--;
    		return ERROR_SUCCESS;
    	case AND:
    	case OR:
    	case EQ:
    	case NE:
    	case GT:
    	case GE:
    	case LE:
    	case LT:
    		if (rootnode->kids[0]){
    			ret = gen_linux_code(rootnode->kids[0]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		gen_dos_push_op(rootnode->kids[0]->type);
    		if (rootnode->kids[1]){
    			ret = gen_linux_code(rootnode->kids[1]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		break;
    	case BOR:
    	case BAND:
    	case BXOR:
    	case ADD:
    	case SUB:
    		if (rootnode->kids[0]){
    			ret = gen_linux_code(rootnode->kids[0]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		gen_dos_push_op(rootnode->kids[0]->type);
    		if (rootnode->kids[1]){
    			ret = gen_linux_code(rootnode->kids[1]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		break;
    	case RSH:
    	case LSH:
    	case DIV:
    	case MUL:
    	case MOD:
    		if (rootnode->kids[0]){
    			ret = gen_linux_code(rootnode->kids[0]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		gen_dos_push_op(rootnode->kids[0]->type);
    		if (rootnode->kids[1]){
    			ret = gen_linux_code(rootnode->kids[1]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		break;
    	case ARRAY:
    		gen_load_address(rootnode->syms[0]);
    		gen_dos_push_op(find_type_by_id(TYPE_INTEGER));
    		if (rootnode->kids[0]){
    			ret = gen_linux_code(rootnode->kids[0]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		break;
    	case SYS:
    		break;
    	case CALL:
    		push_call_stack(rootnode->symtab);
    	default:
    		if (rootnode->kids[0]){
    			ret = gen_linux_code(rootnode->kids[0]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		if (rootnode->kids[1]){
    			ret = gen_linux_code(rootnode->kids[1]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		if (rootnode->kids[2]){
    			ret = gen_linux_code(rootnode->kids[2]);
    			if (ret < 0){
    				gen_level--;
    				return ret;
    			}
    		}
    		break;
	}

	switch (generic(rootnode->op)){
        	case ARRAY:
        		gen_array_factor(rootnode->syms[0]);
        		break;
        	case CNST:
        		gen_factor(rootnode->syms[0]);
        		break;
        	case FIELD:
        		gen_load_address(rootnode->syms[0]);
        		gen_dos_push_op(find_type_by_id(TYPE_INTEGER));
        		gen_record_factor(rootnode->syms[0], rootnode->syms[1]);
        		break;
        	case HEADER:
        		gen_routine_prologue(rootnode->symtab);
        		break;
        	case TAIL:
        		gen_routine_epilogue(rootnode->symtab);
        		break;
        	case NOT:
        		gen_not_factor(rootnode->kids[0]->syms[0]);
        		break;
        	case NEG:
        		gen_negate(rootnode->kids[0]->syms[0]);
        		break;
        	case ASGN:{
        			p = rootnode->kids[0]->syms[0];
        			if (!p)
        				assert(0);
        			if (p->defn == DEF_FUNCT){
        				ptab = find_routine(p->name);
        				if(ptab)
        					gen_function_assign(ptab, rootnode->kids[0]->type->type_id);
        				else{
        					parse_error("Identifier Undeclared .", p->name);
        					gen_level --;
        					return ERROR_UNDECLARE;
        				}
        			}
        			else
        				gen_assign(p, rootnode->kids[0]->type->type_id);
        		}
        		break;
        	case CALL:
        		gen_procedure_call(rootnode->symtab);
        		pop_call_stack();
        		break;
        	case SYS:
        		gen_address = 1;		
        		if ((rootnode->kids[0] == NULL) || (rootnode->kids[0]->kids[0] == NULL) ||
        			(rootnode->kids[0]->kids[1] == NULL)){
        			if (rootnode->kids[0]){
        				if (rootnode->kids[0]->kids[0])
        					gen_linux_code(rootnode->kids[0]->kids[0]);
        				else
        					gen_linux_code(rootnode->kids[0]);
        			}
        			gen_sys_routine(rootnode->u.sys_id, rootnode->kids[0]->type->type_id);
        		}
        		else{
                    #if DEBUG & SYSTEM_CALL_DEBUG
                    {
        				Symtab systab = find_sys_routine(rootnode->u.sys_id);
        				printf("do system call %s with more than one arg.\n", systab->name);
        			}
                    #endif
        			gen_linux_code(rootnode->kids[0]->kids[0]);
        			gen_sys_routine(rootnode->u.sys_id, rootnode->kids[0]->kids[0]->type->type_id);
        			pnode = rootnode->kids[0]->kids[1];
        			while(pnode){
        					if (!pnode->kids[0])
        						break;
        					gen_linux_code(pnode->kids[0]);
        					gen_sys_routine(rootnode->u.sys_id, pnode->kids[0]->type->type_id);
        					pnode = pnode->kids[1];
        			}
        		}
        		gen_address = 0;	
        		break;
        	case COND:
        		gen_cond_jump(rootnode->u.cond.true_or_false, rootnode->u.cond.label);
        		break;
        	case JUMP:
        		gen_jump(rootnode->syms[0]);
        		break;
        	case LABEL:
        		gen_label(rootnode->syms[0]);
        		break;
        	case INCR:
        		assert(rootnode->kids[0]->syms[0]);
        		gen_incr(rootnode->kids[0]->syms[0]);
        		break;
        	case DECR:
        		assert(rootnode->kids[0]->syms[0]);
        		gen_decr(rootnode->kids[0]->syms[0]);
        		break;
        	case LOAD:
        		if (rootnode->kids[0] == NULL){
        			gen_factor(rootnode->syms[0]);
        		}
        		else if (generic(rootnode->kids[0]->op) == ARRAY){
        			gen_load_value(rootnode->kids[0]->syms[0]);
        		}
        		else
        			gen_load_field(rootnode->kids[0]->syms[1]);
        		break;
        	case EQ:
        	case NE:
        	case GT:
        	case GE:
        	case LE:
        	case LT:
        		gen_expression(rootnode->kids[1]->type, rootnode->op);
        		break;
        	case ADD:
        	case SUB:
        	case OR:
        		gen_expr(rootnode->kids[1]->type, rootnode->op);
        		break;
        	case AND:
        	case RSH:
        	case LSH:
        	case BOR:
        	case BAND:
        	case BXOR:
        	case DIV:
        	case MUL:
        	case MOD:
        		gen_term(rootnode->kids[1]->type, rootnode->op);
        		break;
        	case BLOCKBEG:
        	case BLOCKEND:
        		break;
        	case ADDRG:
        		if (gen_address)
        			gen_load_address(rootnode->syms[0]);
        		break;
        	default:
        		assert(0);
        		break;
	}
	gen_level--;
	return ret;
}

static int functionprocess(List dags){
	List cp = dags->link;
	int ret = 0;
	for (; cp; cp = cp->link){
		if ((ret = gen_linux_code((Node)(cp->x))) < 0){
			parse_error("Generate Code Error.","");
			return ret;
		}
	}
	return ret;
}


Interface x86_linux_interface = {
		{1, 1, 0},		// char
		{2, 2, 0},		// short
		{4, 4, 0},		// int
		{4, 4, 0},		// float
		{8, 8, 0},		// double
		{4, 4, 0},		// pointer
		{4, 4, 0},		// struct
		programbegin,
		programend,
		mainbegin,
		mainend,
		defaddress,
		blockbegin,
		blockend,
		globalvariable,
		localvariable,
		deflabel,
		defconst,
		allocspace,
		marknode,
		gen_linux_code,
		functionprocess,
		defexport,
		defimport,
};

