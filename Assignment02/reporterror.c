#include <stdio.h>
// PrintError - 에러케이스별로 Error를 출력하는 함수

void PrintError(ERRORtypes err) {
	switch (err) {
	case overst: // String Table 사이즈가 오버플로우인 경우
		printf("...Error...		OVERFLOW");
		PrintHStable();
		exit(0); // 종료
		break;

	case illsp: // input이 illegal seperator인 경우 (시작이 문자, 숫자, seperator가 모두 아닌 경우)
		printf("...Error...		%c is illegal seperator \n", input);
		break;

	case illid:
		printf("...Error...		"); // input이 illegal identifier인 경우
		while (input != EOF && (isLetter(input) || isDigit(input))) {
			printf("%c", input);
			input = fgetc(fp);
		}
		printf("		start with digit \n"); // 숫자로 시작
		break;

	case overlen: // identifier가 10자 초과인 경우
		printf("...Error...		");
		for (i = nextid; i < nextfree; i++) {
			printf("%c", ST[i]);
		}
		printf("		too long identifier \n");
		break;

	case illid2: // 허용되지 않은 문자가 나타난 경우 
		printf("...Error...		");
		for (i = nextid; i < nextfree; i++) {
			printf("%c", ST[i]);
			if (!(isLetter(ST[i]) || isDigit(ST[i]))) illid2var = i;
		}
		printf("	%c is not allowed\n", ST[illid2var]);
		break;
	}
}