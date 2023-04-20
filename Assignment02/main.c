#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"
#include "symtable.c"
extern yylex();
extern char* yytext;


void main()
{
	enum tokentypes tn;
	const char* tokennames[] = { "TEOF", "TCONST", "TELSE", "TIF", "TINT", "TRETURN", "TVOID", "TWHILE",
		"TEQUAL", "TNOTEQU", "TLE, SSE", "TGREATE", "TAND", "TOR", "TINC", "TDEC",
		"TADDASSIGN", "TSUBASSIGN", "TMULASSIGN", "TDIVASSIGN", "TMODASSIGN",
		"TIDENT", "TNUMBER" };

	while ((tn = yylex()) != TEOF) {
		printf("%s			", tokennames[tn]);
		switch (tn) {
		case TCONST: printf("const\n"); break;
		case TELSE: printf("else\n"); break;
		case TIF: printf("if\n");	break;
		case TINT: printf("int\n");	break;
		case TRETURN: printf("return\n"); break;
		case TVOID: printf("void\n"); break;
		case TWHILE: printf("while\n"); break;
		case TEQUAL: printf("equal\n"); break;
		case TNOTEQU: printf("not equal\n"); break;
		case TLESSE: printf("less than\n"); break;
		case TGREATE: printf("greater than\n"); break;
		case TAND: printf("and\n"); break;
		case TOR: printf("or\n"); break;
		case TINC: printf("increase\n"); break;
		case TDEC: printf("decrease'n"); break;
		case TADDASSIGN: printf("add assign\n"); break;
		case TSUBASSIGN: printf("subtract assign\n"); break;
		case TMULASSIGN: printf("multiply assign\n"); break;
		case TDIVASSIGN: printf("divide assign\n"); break;
		case TMODASSIGN: printf("mod assign\n"); break;
		case TIDENT: printf("%s\n", yytext); break;
		case TNUMBER: printf("%d\n", atoi(yytext)); break;
		}
	}
}