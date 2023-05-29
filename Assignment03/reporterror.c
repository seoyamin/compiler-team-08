/*
* reporterror.c - token error와 parse error 출력 
* programmer - 김민서, 정은비, 최민교
* date - 05/30/2023
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "glob.h"

// Letter, Digit 여부 판단
#define isLetter(x) (((x) >= 'a' && (x) <='z') ||((x) >= 'A' && (x) <='Z') || ((x)=='_'))
#define isDigit(x) (((x) >= '0' && (x) <='9'))

// printTokenError - Token 에러 케이스 별로 오류 메시지를 출력
void printTokenError(const char* input) {
	cErrors++;	// error 개수 증가
	int len = strlen(input);

	// error type 판단
	if (len > 10)
		identifier_et = OVERLEN;				// [case 1] identifier가 10자 초과인 경우 -> overlen
	else if (isDigit(input[0]))					// [case 2] 숫자로 시작하는 경우 -> illid
		identifier_et = ILLID;
	else
		identifier_et = ILLCHAR;				// [case 3] 특수문자가 존재하는 경우 -> illchar

	// error message 출력
	printf(" [TOKEN ERROR] ");
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

// printParseError - Parse 에러 케이스 별로 오류 메시지 출력
void printParseError(char* non_terminal, enum parseerrortypes et)
{
	cErrors++;	// error 개수 증가

	// error message 출력
	printf(" [PARSE ERROR] ");
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
	case NO_COLON:
		printf("no colon");
		break;
	case NO_DCL:
		printf("no declarator");
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
	case NO_RIGHT_SIDE_EXP:
		printf("no right side expression");
		break;
	case NO_EXP_OR_NO_SEMI:
		printf("no expression or no semicolon");
		break;
	case NO_NUM_OR_NO_RIGHTBRACKET:
		printf("no number or no right bracket");
		break;
	}
	printf("  (%s, line % d)\n", non_terminal, currlinenum - 1);
}
