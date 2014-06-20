#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _MSDOS_
#include <io.h>
#endif

#ifdef __linux__
#include <unistd.h>
#endif

#include "common.h"

FILE *codfp, *datfp, *errfp;
FILE *ifp;
extern FILE *yyin;

extern int line_no;
extern int line_pos;


char pasname[FILE_NAME_LEN];
char datname[FILE_NAME_LEN];
char codname[FILE_NAME_LEN];
char errname[FILE_NAME_LEN];

static char buffer[128];
void init(char *);
void emit_asm();
void clear();
void signup();

int yyparse();

int dump_source = 0;
int dump_ast = 0;
int dump_dag = 0;
int dump_asm = 0;
int dump_token = 0;

void print_result(char *);

extern Interface x86_dos_interface;
extern Interface x86_linux_interface;
extern union header *arena[LASTARENA];

Interface *IR = &x86_linux_interface;

void init_spl()
{
	memset(arena, 0, sizeof(arena));
	signup();
}

void prepare_file(char *fname)
{
    char *p;

    get_keytable_size();

    for (p = fname; *p; p++)
        *p = tolower(*p);

    if (strstr(fname, ".pas"))
    {
        for (; p > fname; p--)
        {
            if (*p == '.')
                break;

        }
        *p = '\0';
    }

    snprintf(pasname, sizeof(pasname), "%s.pas", fname);

#ifdef GENERATE_AST
    	snprintf(datname, sizeof(datname), "%s.s", fname);
#else
   	snprintf(datname, sizeof(datname), "%s.asm", fname);
#endif

    snprintf(codname, sizeof(codname), "%s.cod", fname);
    snprintf(errname, sizeof(errname), "%s.err", fname);

    ifp = fopen(pasname, "rt");
    if (!ifp)
    {
        printf("\nCan't open \"%s\"\n", pasname);
        exit(1);
    }

    yyin = ifp;

    line_no = 1;
    line_pos = 0;

    codfp = fopen(codname, "wt");
    datfp = fopen(datname, "wt");
    errfp = fopen(errname, "wt");

    if (!codfp || !datfp || !errfp)
    {
        printf("File open error!\n");
        exit(1);
    }
}

static void finalize()
{
    	/* clear all memory. */
   	deallocate(PERM);
	deallocate(FUNC);
	deallocate(STMT);
	deallocate(TREE);
	deallocate(DAG);
	deallocate(LASTARENA);
}


int main(int argc, char **argv)
{
    int dargc;
    char **arg, **dargv;


    if (argc == 1)
    {
        printf("\nUsage :%s [-d stad] filename[.pas]\n\n", argv[0]);
        return 1;
    }

	init_spl();

    arg = argv + 1;
    dargc = 0;
    dargv = malloc(argc * sizeof(char *));

    /*
     * arguments not recognized by main is pased to 
     * target program_begin
     */
    while(arg)
    {
        if (**(arg) == '-')
        {
            if (arg[0][1] == 'd')
            {
                    char *p = arg[1];
                    while (*p)
                    {
                        switch(*p++)
                        {
                        case 's':
                            dump_source = 1;
                            break;
                        case 'a':
                            dump_ast = 1;
                            break;
                        case 't':
                            dump_token = 1;
                            break;
                        case 'd':
                            dump_dag = 1;
                            break;
                        default:
                            printf("Unkown dump option %c.\n", *(p - 1));
                            break;
                        }
                    }
		arg++;
                arg++;
                break;
            }
            else {
                dargv[dargc++] = *arg++;
                dargv[dargc++] = *arg++;
                break;
            }
        }
        else
        {
            prepare_file(arg[0]);
            break;
        }
    }

    global_env.u.program.argc = dargc;
    global_env.u.program.argv = dargv;

#ifndef GENERATE_AST
	IR = &x86_dos_interface;
#endif

    yyparse();
    fclose(ifp);

    if (!err_occur())
    {
        emit_asm();
        print_result(pasname);
        return 0;
    }
    else
    {
        clear();
        print_result(pasname);
        return 1;
    }

    finalize();

    free(dargv);

    return 0;
}

void emit_asm()
{
    fclose(codfp);
    codfp = fopen(codname, "rt");
    fgets(buffer, sizeof(buffer), codfp);
    while(!feof(codfp))
    {
        fputs(buffer, datfp);
        fgets(buffer, sizeof(buffer), codfp);
    }

    fclose(codfp);
    fclose(datfp);
    fclose(errfp);
    unlink(codname);
    unlink(errname);
}

void clear()
{
    fclose(codfp);
    fclose(datfp);
    fclose(errfp);
    unlink(codname);
    unlink(datname);
}

void signup()
{
    printf("\n");
    printf("Compiling...");
}

