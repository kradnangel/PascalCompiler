%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include _YTAB_H_

int Keytable_size = 52;
extern int line_no;

extern YYSTYPE	yylval;

static int check(char *lex);

%}

letter		[_a-zA-Z]
alnum		[_a-zA-Z0-9]
dec		[0-9]
hex		[0-9a-fA-F]
oct		[0-7]

%%
"{"		{
			int c;
			while ((c = input())) {
				if (c == '}')
					break;
				else if (c == '\n')
					line_no ++; 
				if (c == EOF) {
					yyerror("Unexpected EOF. '{' '}' mismatch");
					break;
				}
			}
		}

[1-9]+{dec}*				{
						yylval.num = stoi(yytext, 10);
						printf("token: cINTEGER, yylval.num = %d.\n", yylval.num);
						return cINTEGER;
					}
					
0{oct}*					{
						yylval.num = stoi(yytext, 8);
						printf("token: cINTEGER, yylval.num = %d.\n", yylval.num);
						return cINTEGER;
					}
					
0(x|X){hex}+				{
						yylval.num = stoi(yytext, 16);
						printf("token: cINTEGER, yylval.num = %d.\n", yylval.num);
						return cINTEGER;
					}

{dec}+(\.{dec}+)?([E|e][+\-]?{dec}+)?	{
						strncpy(yylval.p_char, yytext, NAME_LEN);
						printf("token: cREAL, yylval.p_char = %s.\n", yylval.p_char);
						return cREAL;
					}
					
'([^']|\")'				{
						strncpy(yylval.p_char, yytext, NAME_LEN);
						printf("token: cCHAR, yylval.p_char = %s.\n", yylval.p_char);
						return cCHAR;
					}
					
\'(\\.|[^\'])(\\.|[^\'])+\'		{
						strncpy(yylval.p_char, yytext, NAME_LEN);
						printf("token: cSTRING, yylval.p_char = %s.\n", yylval.p_char);
						return cSTRING;
					}
					
"("					{ printf("token: oLP.\n");return oLP; }
")"					{ printf("token: oRP.\n");return oRP; }
"["					{ printf("token: oLB.\n");return oLB; }
"]"					{ printf("token: oRB.\n");return oRB; }
"+"					{ printf("token: oPLUS.\n");return oPLUS; }
"-"					{ printf("token: oMINUS.\n");return oMINUS; }
"*"					{ printf("token: oMUL.\n");return oMUL; }
"/"					{ printf("token: oDIV.\n");return oDIV; }
":="					{ printf("token: oASSIGN.\n");return oASSIGN; }
"="					{ printf("token: oEQUAL.\n");return oEQUAL; }
"<"					{ printf("token: oLT.\n");return oLT; }
">"					{ printf("token: oGT.\n");return oGT; }
"<="					{ printf("token: oLE.\n");return oLE; }
">="					{ printf("token: oGE.\n");return oGE; }
"<>"					{ printf("token: oUNEQU.\n");return oUNEQU; }
","					{ printf("token: oCOMMA.\n");return oCOMMA; }
";"					{ printf("token: oSEMI.\n");return oSEMI; }
":"					{ printf("token: oCOLON.\n");return oCOLON; }
"'"					{ printf("token: oQUOTE.\n");return oQUOTE; }
".."					{ printf("token: oDOTDOT.\n");return oDOTDOT; }
"."					{ printf("token: oDOT.\n");return oDOT; }
"^"					{ printf("token: oARROW.\n");return oARROW; }
{letter}{alnum}*			{
						strncpy(yylval.p_char, yytext, NAME_LEN);
						return check(yytext);
					}
"\n"					{	line_no++;}
.					{}

%%

KEYENTRY Keytable[] = {
{"abs",		SYS_FUNCT,	fABS, 	TYPE_INTEGER, TYPE_INTEGER },
{"and",		kAND,		KEYWORD, 0, 0 },
{"array",	kARRAY, 	KEYWORD, 0, 0 },
{"begin",	kBEGIN, 	KEYWORD, 0, 0 },
{"boolean", 	SYS_TYPE,	tBOOLEAN, 0, 0 },
{"case",	kCASE, 		KEYWORD, 0, 0 },
{"char",	SYS_TYPE,	tCHAR,	0, 0 },
{"chr",		SYS_FUNCT, 	fCHR, 	TYPE_CHAR, TYPE_CHAR },
{"const",	kCONST,		KEYWORD, 0, 0 },
{"div", 	kDIV,		KEYWORD, 0, 0 },
{"do",		kDO, 		KEYWORD, 0, 0 },
{"downto",	kDOWNTO,	KEYWORD, 0, 0 },
{"else",	kELSE,		KEYWORD, 0, 0 },
{"end", 	kEND, 		KEYWORD, 0, 0 },
{"false",	SYS_CON,	cFALSE, 0, 0 },
{"for",		kFOR,		KEYWORD, 0, 0 },
{"function", 	kFUNCTION, 	KEYWORD, 0, 0 },
{"goto",	kGOTO,		KEYWORD, 0, 0 },
{"if",		kIF, 		KEYWORD, 0, 0 },
{"in", 		kIN,		KEYWORD, 0, 0 },
{"integer", 	SYS_TYPE,	tINTEGER,	0, 0 },
{"label",	kLABEL,		KEYWORD, 0, 0 },
{"maxint", 	SYS_CON,	cMAXINT, 0, 0 },
{"mod",		kMOD,		KEYWORD, 0, 0 },
{"not", 	kNOT,		KEYWORD, 0, 0 },
{"odd",		SYS_FUNCT,	fODD, 	TYPE_INTEGER, TYPE_BOOLEAN },
{"of",		kOF,		KEYWORD, 0, 0 },
{"or",		kOR,		KEYWORD, 0, 0 },
{"ord",		SYS_FUNCT,	fORD, 	TYPE_INTEGER, TYPE_INTEGER },
{"packed",	kPACKED,	KEYWORD, 0, 0 },
{"pred",	SYS_FUNCT,	fPRED, 	TYPE_INTEGER, TYPE_INTEGER },
{"procedure", 	kPROCEDURE,	KEYWORD, 0, 0 },
{"program", 	kPROGRAM, 	KEYWORD, 0, 0 },
{"read",	pREAD,		pREAD, 0, 0 },
{"readln", 	pREAD,		pREADLN, 0, 0 },
{"real",	SYS_TYPE,	KEYWORD, 0, 0 },
{"record", 	kRECORD,	KEYWORD, 0, 0 },
{"repeat",	kREPEAT,	KEYWORD, 0, 0 },
{"set", 	kSET, 		KEYWORD, 0, 0 },
{"sqr",		SYS_FUNCT,	fSQR,	TYPE_INTEGER, TYPE_INTEGER },
{"sqrt",	SYS_FUNCT,	fSQRT,	TYPE_INTEGER, TYPE_INTEGER },
{"succ",	SYS_FUNCT,	fSUCC,	TYPE_INTEGER, TYPE_INTEGER },
{"then",	kTHEN,		KEYWORD, 0, 0 },
{"to",		kTO,		KEYWORD, 0, 0 },
{"true",	SYS_CON,	cTRUE,	0, 0 },
{"type",	kTYPE,		KEYWORD, 0, 0 },
{"until",	kUNTIL,		KEYWORD, 0, 0 },
{"var",		kVAR,		KEYWORD, 0, 0 },
{"while",	kWHILE,		KEYWORD, 0, 0 },
{"with",	kWITH, 		KEYWORD, 0, 0 },
{"write",	SYS_PROC,	pWRITE, 0, 0 },
{"writeln",	SYS_PROC,	pWRITELN, 0, 0 },
{"----",	LAST_ENTRY,	KEYWORD, 0, 0 },
};

static int check(char *lex)
{
	int mid = 0;
	char *p;
	
	for (p = lex; *p; p++)
		*p = tolower(*p);

	while (mid++ <= Keytable_size)
	{
		if (!strcmp(Keytable[mid].name, lex))
		{
			printf("token: keyword - %s ", Keytable[mid].name);
			if (Keytable[mid].key == SYS_FUNCT
				|| Keytable[mid].key == SYS_PROC) {
				yylval.p_lex = &Keytable[mid];
				printf(",yylval.p_lex = %s .\n", yylval.p_lex);
			}
			else if (Keytable[mid].key == SYS_CON) {
				yylval.num = Keytable[mid].attr;
				printf(",yylval.p_num = %d .\n", yylval.num);
			}
			else if (Keytable[mid].key == SYS_TYPE) {
				strncpy(yylval.p_char, yytext, NAME_LEN);
				printf(",yylval.p_char = %s .\n", yylval.p_char);
			}
			else printf("\n");
			return Keytable[mid].key;
		}
	}

	printf("token: yNAME, yylval.p_char = %s.\n", yylval.p_char);
	
	return yNAME;
}

int yywrap() 
{
	return 1;
}

