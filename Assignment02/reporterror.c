#include <stdio.h>
// PrintError - �������̽����� Error�� ����ϴ� �Լ�

void PrintError(ERRORtypes err) {
	switch (err) {
	case overst: // String Table ����� �����÷ο��� ���
		printf("...Error...		OVERFLOW");
		PrintHStable();
		exit(0); // ����
		break;

	case illsp: // input�� illegal seperator�� ��� (������ ����, ����, seperator�� ��� �ƴ� ���)
		printf("...Error...		%c is illegal seperator \n", input);
		break;

	case illid:
		printf("...Error...		"); // input�� illegal identifier�� ���
		while (input != EOF && (isLetter(input) || isDigit(input))) {
			printf("%c", input);
			input = fgetc(fp);
		}
		printf("		start with digit \n"); // ���ڷ� ����
		break;

	case overlen: // identifier�� 10�� �ʰ��� ���
		printf("...Error...		");
		for (i = nextid; i < nextfree; i++) {
			printf("%c", ST[i]);
		}
		printf("		too long identifier \n");
		break;

	case illid2: // ������ ���� ���ڰ� ��Ÿ�� ��� 
		printf("...Error...		");
		for (i = nextid; i < nextfree; i++) {
			printf("%c", ST[i]);
			if (!(isLetter(ST[i]) || isDigit(ST[i]))) illid2var = i;
		}
		printf("	%c is not allowed\n", ST[illid2var]);
		break;
	}
}