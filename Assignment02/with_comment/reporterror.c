#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "glob.h"

// Letter, Digit 여부 판단
#define isLetter(x) (((x) >= 'a' && (x) <='z') ||((x) >= 'A' && (x) <='Z') || ((x)=='_'))
#define isDigit(x) (((x) >= '0' && (x) <='9'))

// 에러 타입 enum
enum errortypes {
	OVERLEN, ILLCHAR, ILLID
};

// reporterror - 에러케이스 별로 Error를 출력하는 함수

void reporterror(const char* input) {
	cErrors++;
	int len = strlen(input);

	// error type 판단
	if (len > 10)
		et = OVERLEN;				// [case 1] identifier가 10자 초과인 경우 -> overlen
	else if (isDigit(input[0]))		// [case 2] 숫자로 시작하는 경우 -> illid
		et = ILLID;
	else 
		et = ILLCHAR;				// [case 3] 특수문자가 존재하는 경우 -> illchar
	

	printf("%d			**Error**					", linenum);

	// error message 출력
	switch (et) {
	case OVERLEN: 
		printf("%s Too Long \n", input);
		break;
	case ILLID: 
		printf("%s Illegal IDENT \n", input);
		break;
	case ILLCHAR:
		for (int i = 0; i < len; i++) {
			if (!(isDigit(input[i]) || isLetter(input[i])))
				printf("%c ", input[i]);	// illegal character 출력
		}
		printf(" Illegal Character\n");
		break;
	}
}