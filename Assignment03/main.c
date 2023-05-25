/*
* main.c - 각 token에 대한 정보를 출력
* programmer – 김민서, 정은비, 최민교
* date – 30/05/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tn.h"
#include "glob.h"
extern yylex();
extern int yyparse();
extern STindex();
extern ComputeHS();
extern LookupHS();
extern PrintHStable();
extern char* yytext;

unsigned int cErrors = 0;       // 발생한 에러의 횟수
unsigned int currlinenum = 1;       // 현재 토큰이 선언된 위치의 line number


// printtoken - 토큰 정보를 출력하는 함수

void printtoken(enum tokentypes tn) {

	// Line number 출력
	printf("%d		", currlinenum);

	// Token Type 출력
	switch (tn) {
	case TCONST: printf("const\t\t\t"); break;
	case TELSE: printf("else\t\t\t"); break;
	case TIF: printf("if\t\t\t");	break;
	case TINT: printf("int\t\t\t");	break;
	case TRETURN: printf("return\t\t\t"); break;
	case TVOID: printf("void\t\t\t"); break;
	case TWHILE: printf("while\t\t\t"); break;
	case TADD: printf("add\t\t\t"); break;
	case TSUB: printf("subtract\t\t"); break;
	case TMUL: printf("multiply\t\t"); break;
	case TDIV: printf("divide\t\t\t"); break;
	case TMOD: printf("modulo\t\t\t"); break;
	case TASSIGN: printf("assign\t\t\t"); break;
	case TADDASSIGN: printf("add assign\t\t"); break;
	case TSUBASSIGN: printf("subtract assign\t\t"); break;
	case TMULASSIGN: printf("multiply assign\t\t"); break;
	case TDIVASSIGN: printf("divide assign\t\t"); break;
	case TMODASSIGN: printf("modulo assign\t\t"); break;
	case TNOT: printf("not\t\t\t"); break;
	case TAND: printf("and\t\t\t"); break;
	case TOR: printf("or\t\t\t");  break;
	case TEQUAL: printf("equal to\t\t"); break;
	case TNOTEQU: printf("not equal to\t\t"); break;
	case TLESS: printf("less than\t\t"); break;
	case TGREAT: printf("greater than\t\t"); break;
	case TLESSE: printf("less than or equal to\t"); break;
	case TGREATE: printf("greater than or equal to"); break;
	case TINC: printf("increase\t\t"); break;
	case TDEC: printf("decrease\t\t"); break;
	case TLEFTPAR: printf("left parenthesis\t"); break;
	case TRIGHTPAR: printf("right parenthesis\t"); break;
	case TLEFTBRACE: printf("left brace\t\t"); break;
	case TRIGHTBRACE: printf("right brace\t\t"); break;
	case TLEFTBRACKET: printf("left bracket\t\t"); break;
	case TRIGHTBRACKET: printf("right bracket\t\t"); break;
	case TSEMI: printf("semicolon\t\t"); break;
	case TCOLON: printf("colon\t\t\t"); break;
	case TIDENT: printf("ident			 %d", currid); break;	  // Identifier인 경우 ST-index도 출력
	case TNUMBER: printf("number\t\t\t\t\t\t"); break;
	}

	// Token 값 출력
	if (tn == TNUMBER)
		printf("%d \n", atoi(yytext));
	else
		printf("\t\t\t%s \n", yytext);
}


// main 함수 

void main()
{
	enum tokentypes tn;  // 토큰 타입

	printf("*** MiniC parsing begins\n");
	yyparse();
	printf("Parsing ends. ***\n");
	//printf("%d error(s) detected\n", cErrors);

	PrintHStable();
	printf("\n[1983024 Mingyo Choi, 2017007 Minseo Kim, 2173109 Eunbi Jeong]\n");


	//// 헤더 출력 (Line number, Token Type, ST-index, Token)
	//printf("Line number	Token type		 ST-index	        Token\n");

	//// 에러가 아닌 경우, Token 정보 출력
	//while ((tn = yylex()) != TEOF) {
	//	if (tn != TERROR)
	//		printtoken(tn);
	//}

	//// Error 등장 횟수 출력
	//if (cErrors == 0)
	//	printf("\nNo errors detected ");
	//else printf("\n%d errors detected", cErrors);

	//// 학번 출력
	//printf("\n\n[1983024 Choi mingyo, 2017007 Kim minseo, 2173109 Jeong Eunbi]\n");
}
