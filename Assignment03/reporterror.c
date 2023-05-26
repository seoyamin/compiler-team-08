/*
* reporterror.c - 각 token에 대한 error 메시지 출력
* programmer - 김민서, 정은비, 최민교
* date - 04/26/2023
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "glob.h"


// Letter, Digit 여부 판단
#define isLetter(x) (((x) >= 'a' && (x) <='z') ||((x) >= 'A' && (x) <='Z') || ((x)=='_'))
#define isDigit(x) (((x) >= '0' && (x) <='9'))

// reporterror - 에러케이스 별로 Error를 출력하는 함수
void reporterror(const char* input) {
	cErrors++;
	int len = strlen(input);

	// error type 판단
	if (len > 10)
		identifier_et = OVERLEN;				// [case 1] identifier가 10자 초과인 경우 -> overlen
	else if (isDigit(input[0]))		// [case 2] 숫자로 시작하는 경우 -> illid
		identifier_et = ILLID;
	else
		identifier_et = ILLCHAR;				// [case 3] 특수문자가 존재하는 경우 -> illchar


	printf("[LEX ERROR]  ");

	// error message 출력
	switch (identifier_et) {
	case OVERLEN:
		printf("\"%s\" is too long", input);
		break;
	case ILLID:
		printf("\"%s\" is an illegal identifier", input);
		break;
	case ILLCHAR:
		for (int i = 0; i < len; i++) {
			if (!(isDigit(input[i]) || isLetter(input[i])))
				printf("\"%c\" ", input[i]);	// illegal character 출력
		}
		printf(" is illegal character");
		break;
	}
	printf(" (line %d)\n", currlinenum);
}

void yyerror(char* s) {}

void printerror(char* non_terminal, enum errortypes et)
{
	cErrors++;
	printf("[PARSE ERROR] ");

	// error message 출력
	switch (et) {
	case NO_SEMI:
		printf("no semicolon");
		break;
	case NO_EXP:
		printf("no expression");
		break;
	case NO_NUM:
		printf("no number");
		break;
	case NO_LEFTBRACKET:
		printf("no left bracket");
		break;
	case NO_RIGHTBRACKET:
		printf("no right bracket");
		break;
	case NO_LEFTBRACE:
		printf("no left brace");
		break;
	case NO_RIGHTBRACE:
		printf("no right brace");
		break;
	case NO_LEFTPAR:
		printf("no left parenthesis");
		break;
	case NO_RIGHTPAR:
		printf("no right parenthesis");
		break;
	}

	printf("  (%s, line % d)\n", non_terminal, currlinenum - 1);
}
