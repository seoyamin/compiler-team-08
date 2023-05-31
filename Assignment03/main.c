/*
* main.c - Modified MiniC의 구문 분석을 위한 main 함수
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

unsigned int cErrors = 0;		// 발생한 에러의 횟수 초기화
unsigned int currlinenum = 1;       	// 현재 토큰이 선언된 위치의 line number 초기화
int cReturntype = -1;       		// function name의 return type 초기화

// main 함수 
void main()
{
	printf("\n************************** start of parser **************************** \n\n");
	yyparse();
	printf("\n*************************** end of parser ***************************** \n\n");

	printf("-> %d errors detected\n", cErrors);	// 에러가 발생한 횟수 출력
	PrintHStable();									// Hash Table 출력
	printf("\n[1983024 Mingyo Choi, 2017007 Minseo Kim, 2173109 Eunbi Jeong]\n");
}
