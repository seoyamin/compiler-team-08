#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "glob.h"

// Letter, Digit ���� �Ǵ�
#define isLetter(x) (((x) >= 'a' && (x) <='z') ||((x) >= 'A' && (x) <='Z') || ((x)=='_'))
#define isDigit(x) (((x) >= '0' && (x) <='9'))

// ���� Ÿ�� enum
enum errortypes {
	OVERLEN, ILLCHAR, ILLID
};

// reporterror - �������̽� ���� Error�� ����ϴ� �Լ�

void reporterror(const char* input) {
	cErrors++;
	int len = strlen(input);

	// error type �Ǵ�
	if (len > 10)
		et = OVERLEN;				// [case 1] identifier�� 10�� �ʰ��� ��� -> overlen
	else if (isDigit(input[0]))		// [case 2] ���ڷ� �����ϴ� ��� -> illid
		et = ILLID;
	else 
		et = ILLCHAR;				// [case 3] Ư�����ڰ� �����ϴ� ��� -> illchar
	

	printf("%d			**Error**					", linenum);

	// error message ���
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
				printf("%c ", input[i]);	// illegal character ���
		}
		printf(" Illegal Character\n");
		break;
	}
}