#define   SYMTABC
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <ctype.h>
#include  "common.h"
#include  "error.h"
#include  "symtab.h"

#include  _YTAB_H_
#define  SYMTAB_QUEUE_SIZE 128
#define  SYMTAB_STACK_SIZE 64
int const_index;

int var_index;
int arg_index;
int cur_level;
int routine_id;
int last_symtab=0;
int align(int);
int symtab_tos = SYMTAB_STACK_SIZE - 1;
symtab *symtab_queue[SYMTAB_QUEUE_SIZE];
symtab *Global_symtab;
symtab *System_symtab[MAX_SYS_ROUTINE];
symtab *symtab_stack[SYMTAB_STACK_SIZE];
extern int Keytable_size;

int align(int bytes)
{
    while (bytes % 2)
        bytes++;
    return bytes;
}

void enter_symtab_queue(symtab *tab)
{
    if (last_symtab < SYMTAB_QUEUE_SIZE)
        symtab_queue[last_symtab ++] = tab;
}

symtab *new_symtab(symtab *parent)
{
    symtab *p;

    /* p = (symtab *)malloc(sizeof(symtab)); */
    NEW0(p, PERM);

    if(!p)
        internal_error("insufficient memory.");
    p->parent = parent;
    p->args_size = 0;
    p->args = NULL;
    p->localtab = NULL;
    p->locals = NULL;
    p->type_link = NULL;
    if(parent)
    {
        p->level = parent->level+1;
    }
    else
    {
        routine_id = 0;
        Global_symtab = p;
        Cur_level = 0;
        p->level = 0;
    }
    p->defn = DEF_UNKNOWN;
    p->type = find_type_by_id(TYPE_VOID);
    p->id = routine_id++;
    p->local_size = 0;
    enter_symtab_queue(p);
    return p;
}

symbol *new_symbol(char *name, int defn, int typeid)
{
    symbol *p;
    static int imp_index = 0;

    /* p = (symbol *)malloc(sizeof(symbol)); */
    NEW0(p, PERM);

    if(!p)
        internal_error("insuffceent memory.");

    if(!strcmp(name,"$$$"))
        sprintf(p->name,"z%d",++imp_index);
    else
        strncpy(p->name, name, NAME_LEN);
    p->rname[0]= '\0';
    p->defn = defn;
    p->type = find_type_by_id(typeid);
    p->offset = 0;
    p->next = NULL;
    p->lchild = p->rchild = NULL;
    p->tab = NULL;
    p->type_link = NULL;
    return p;

}
symbol *clone_symbol(symbol *origin)
{
    symbol *p;
    if(!origin)
        return NULL;
    /* p = (symbol *)malloc(sizeof(symbol)); */
    NEW0(p, PERM);
    if(!p)
        internal_error("insuffident memory.");
    strncpy(p->name,origin->name,NAME_LEN);
    strncpy(p->name,origin->rname,LABEL_LEN);
    p->defn = origin->defn;
    p->type = origin->type;
    p->offset = 0;
    p->next = NULL;
    p->tab = NULL;
    if (p->type->type_id == TYPE_STRING)
        p->v.s = strdup(origin->v.s);
    else
        p->v.f = origin->v.f;
    return p;
}

symbol *clone_symbol_list(symbol *head)
{
    symbol *new_list;
    symbol *p;
    symbol *q;
    if(!head)
        return NULL;
    q = head;
    new_list = p = clone_symbol(q);
    q = p->next;
    while(q)
    {
        p->next = clone_symbol(q);
        p = p->next;
        q = q->next;
    }
    p->next = NULL;
    return new_list;
}

symbol *reverse_parameters(symtab *ptab)
{
    symbol *p,*q;
    symbol *new_list = NULL;
    for(p = ptab->args;p;)
    {
        q = p;
        p = p->next;
        q->next = new_list;
        new_list = q;
    }
    ptab->args = new_list;
    return ptab->args;
}

void add_symbol_to_table(symtab*tab, symbol *sym)
{
    switch(sym->defn)
    {
    case DEF_FUNCT:
    case DEF_PROC:
    case DEF_VAR:
    case DEF_CONST:
        add_local_to_table(tab, sym);
        break;
    case DEF_VARPARA:
    case DEF_VALPARA:
        add_args_to_table(tab,sym);
        break;
    case DEF_PROG:
    default:
        break;
    }
}

void add_var_to_localtab(symtab *tab,symbol *sym)
{
    symbol *p;
    int i;

    if (!tab || !sym)
        return;

    if(!tab->localtab)
    {
        tab->localtab = sym;
        return;
    }
    p = tab->localtab;
    while(1)
    {
        i = strcmp(sym->name, p->name);
        if(i > 0)
        {
            if(p->rchild)
                p = p->rchild;
            else
            {
                p->rchild = sym;
                break;
            }
        }
        else if (i < 0)
        {
            if (p->lchild)
                p = p->lchild;
            else
            {
                p->lchild = sym;
                break;
            }
        }
        else
        {
            parse_error("Duplicate identifier.",
                        sym->name);
            break;
        }
    }
}

void add_local_to_table(symtab *tab,symbol *sym)
{
    if(!tab|| !sym)
        return;
    if(sym->defn == DEF_CONST)
        sprintf(sym->rname, "c%c_%03d",
                sym->name[0],new_index(const));
    else
        sprintf(sym->rname, "v%c_%03d",
                sym->name[0],new_index(var));
    if(tab->level)
    {
        if(tab->defn == DEF_FUNCT
                && sym->defn != DEF_FUNCT)
        {
            sym->offset = tab->local_size + 3 * IR->intmetric.size;
            tab->local_size += align(get_symbol_size(sym));
        }
        else if (tab->defn == DEF_PROC
                 && sym->defn != DEF_PROC)
        {
            sym->offset = tab->local_size + IR->intmetric.size;
            tab->local_size += align(get_symbol_size(sym));
        }
    }

    sym->next = tab->locals;
    tab->locals=sym;
    sym->tab = tab;
    add_var_to_localtab(tab,sym);
}

void add_args_to_table(symtab *tab, symbol *sym)
{
    symbol *p;
    int var_size;
    if(!tab|| !sym)
        return;
    sym->next = tab->args;
    tab->args = sym;
    sym->tab = tab;
    sym->offset = 3 * IR->intmetric.size;
    sprintf(sym->rname,"a%c_%03d",
            sym->name[0],new_index(arg));
    var_size = align(get_symbol_size(sym));
    tab->args_size += var_size;
    for(p = tab->args->next; p; p = p->next)
        p->offset += var_size;
    add_var_to_localtab(tab,sym);
}

extern KEYENTRY Keytable[];

void make_system_symtab()
{
    int i,n;
    symtab *ptab;
    type *pt;

    for(i = 0; i < MAX_SYS_ROUTINE; i++)
        System_symtab[i] = NULL;


    /* System_symtab[0]=
    		(symtab*)malloc(sizeof(symtab));
    */
    NEW0(ptab, PERM);
    System_symtab[0] = ptab;

    if(!System_symtab)
        internal_error("Insuflicent memory. ");

    ptab = System_symtab[0];
    sprintf(ptab->name,"system_table");
    sprintf(ptab->rname, "null");

    ptab->type_link=
        new_system_type(TYPE_INTEGER);
    pt = ptab->type_link;
    pt->next = new_system_type(TYPE_CHAR);
    pt = pt->next;
    pt->next = new_system_type(TYPE_BOOLEAN);
    pt = pt->next;
    pt->next = new_system_type(TYPE_REAL);
    pt = pt->next;
    pt->next = new_system_type(TYPE_VOID);
    pt = pt->next;
    pt->next = new_system_type(TYPE_STRING);
    pt = pt->next;
    pt->next = new_system_type(TYPE_UNKNOWN);
    pt = pt->next;

    push_symtab_stack(ptab);

    ptab->id=-1;
    ptab->level=-1;
    ptab->defn = DEF_UNKNOWN;
    ptab->type = TYPE_UNKNOWN;
    ptab->local_size = 0;
    ptab->args_size = 0;
    ptab->args = NULL;
    ptab->parent = NULL;
    ptab->locals = new_symbol("",DEF_UNKNOWN,
                              TYPE_UNKNOWN);

    n = 1;

    /* for(i = 0 ; i < MAX_SYS_ROUTINE; i++){ */
    for(i = 0 ; i < Keytable_size; i++)
    {
        if(Keytable[i].key == SYS_FUNCT ||
                Keytable[i].key == SYS_PROC )
        {
            System_symtab[n]=
                new_sys_symbol(Keytable[i]);
            n++;
        }
        else if (Keytable[i].key == LAST_ENTRY)
            break;
    }

    pop_symtab_stack();

    ptab->local_size = n;
}

symtab* new_sys_symbol(KEYENTRY entry)
{
    symtab *ptab;
    symbol *p;

    /* ptab = (symtab*)malloc(sizeof(symtab)); */
    NEW0(ptab, PERM);

    if(!ptab)
        internal_error("Insufticent  memoy.");

    strcpy(ptab->name, entry.name);

    sprintf(ptab->rname, "_f_%s",entry.name);

    ptab->id = -entry.attr;
    ptab->level = -1;
    ptab->defn = DEF_FUNCT;
    ptab->type = find_type_by_id(entry.ret_type);
    ptab->local_size = 0;
    ptab->args_size = 0;
    ptab->args = NULL;
    ptab->localtab = NULL;
    ptab->locals = NULL;
    ptab->parent = System_symtab[0];

    /* p = (symbol*)malloc(sizeof(symbol)); */
    NEW0(p, PERM);

    if(!p)
        internal_error("Insufticent memory.");

    strcpy(p->name, ptab->name);
    strcpy(p->rname, ptab->rname);
    p->defn = DEF_FUNCT;
    p->type = find_type_by_id(entry.ret_type);
    p->offset = 0;
    p->next = NULL;
    p->tab = ptab;
    p->type_link = NULL;
    ptab->locals = p;
    if(entry.arg_type)
    {
        /* p = (symbol*)malloc(sizeof(symbol)); */
        NEW0(p, PERM);

        if(!p)
            internal_error("Insufficent memory.");
        strcpy(p->name, "arg");
        p->defn =DEF_VALPARA;
        p->type = find_type_by_id(entry.arg_type);
        add_args_to_table(ptab,p);
    }
    return ptab;
}

int is_symbol(symbol*p,char*name)
{
    if(strcmp(p->name,name))
        return  0;
    return 1;
}

int get_symbol_size(symbol*sym)
{
    switch(sym->type->type_id)
    {
    case TYPE_INTEGER:
        return  IR->intmetric.size;
    case TYPE_CHAR   :
        return  IR->charmetric.size;
    case TYPE_BOOLEAN:
        return  IR->intmetric.size;
    case TYPE_REAL   :
        return  IR->floatmetric.size;

    case TYPE_STRING :
        return size (CHAR)
               *(strlen(sym->v.s) + 1);
    case TYPE_ARRAY  :
        return  get_type_size(
                    sym->type_link);

    case TYPE_RECORD  :
        return get_type_size(
                   sym->type_link);
    case TYPE_UNKNOWN:
        internal_error("Unknown type.");
        break;
    case  TYPE_VOID   :
        return 0;
    default:
        break;
    }
    return 0;
}

symtab *pop_symtab_stack()
{
    if(symtab_tos == SYMTAB_STACK_SIZE)
        internal_error("Symtab stack underflow.");
    return symtab_stack[++symtab_tos];
}
void push_symtab_stack (symtab *tab)
{
    if(symtab_tos== -1)
        internal_error("Symtab stack overflow.");
    else
        symtab_stack[symtab_tos--] = tab;
}

symtab *top_symtab_stack()
{
    return symtab_stack[symtab_tos + 1];
}

symtab *find_routine(char *name)
{
    int i;
    symtab *ptab;
    for(i=0;i<last_symtab;i++)
    {
        ptab = symtab_queue[i];
        if(!strcmp(ptab->name,name))
            return ptab;
    }
    return NULL;
}

symtab *find_sys_routine(int routine_id)
{
    int i;
    for(i = 1; i < System_symtab[0]->local_size; i++)
        if(System_symtab[i]->id == -routine_id)
            return System_symtab[i];
    return NULL;
}

symbol *find_element(symtab *tab,char *name)
{
    symbol *p;
    symtab *ptab = tab;
    type *pt;
    while(ptab)
    {
        for(pt = ptab->type_link; pt; pt = pt->next)
            for(p = pt->first; p; p = p->next)
                if(is_symbol(p,name))
                    return p;
        ptab = ptab->parent;
    }
    return NULL;
}

symbol *find_field(symbol *p,char *name)
{
    type *pt;
    symbol *q;

    if(!p || p->type->type_id != TYPE_RECORD)
        return NULL;
    pt = p->type_link;
    for(q = pt->first; q; q = q->next)
        if(is_symbol(q, name))
            return q;

    return NULL;
}

symbol *find_symbol(symtab *tab,char *name)
{
    symbol *p;
    symtab *ptab = tab;
    type *pt;

    if(!ptab)
    {
        p = System_symtab[0]->locals;
        p->type = TYPE_UNKNOWN;
        return p;
    }
    while(ptab)
    {
        p = find_local_symbol(ptab,name);
        if(p)
            return p;
        for(pt = ptab->type_link; pt; pt=pt->next)
            for(p = pt->first; p; p = p->next)
                if(is_symbol(p,name))
                    return p;
        ptab = ptab->parent;
    }
    return NULL;
}
symbol *find_local_symbol(symtab *tab, char *name)
{
    symbol *p;
    symtab *ptab = tab;
    int i;
    if(!ptab)
        return NULL;
    if(!ptab->localtab)
        return NULL;
    p = ptab->localtab;
    while(p)
    {
        i = strcmp(name, p->name);
        if(i > 0)
            p = p->rchild;
        else if(i < 0)
            p = p->lchild;
        else
            return p;
    }
    return NULL;
}

