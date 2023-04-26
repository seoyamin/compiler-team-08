#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"
extern yylex();
extern STindex();
extern ComputeHS();
extern LookupHS();
extern char* yytext;

unsigned int cErrors = 0;       // �߻��� ������ Ƚ��
unsigned int linenum = 1;       // ����� ���� Line Number


// printtoken - ��ū ������ ����ϴ� �Լ�

void printtoken(enum tokentypes tn) {

	// Line number ���
	printf("%d		", linenum);

	// Token Type ���
	switch (tn){
		case TCONST: printf("const				"); break;
		case TELSE: printf("else 				"); break;
		case TIF: printf("if 				");	break;
		case TINT: printf("int 				");	break;
		case TRETURN: printf("return 				"); break;
		case TVOID: printf("void 				"); break;
		case TWHILE: printf("while 				"); break;

		case TADD: printf("add 				"); break;
		case TSUB: printf("subtract				"); break;
		case TMUL: printf("multiplication				"); break;
		case TDIV: printf("divide				"); break;
		case TMOD: printf("modulo				"); break;

		case TASSIGN: printf("assign				"); break;
		case TADDASSIGN: printf("add assign				"); break;
		case TSUBASSIGN: printf("subtract assign				"); break;
		case TMULASSIGN: printf("multiply assign				"); break;
		case TDIVASSIGN: printf("divide assign					"); break;
		case TMODASSIGN: printf("modulo assign				"); break;

		case TNOT: printf("not				");	break;
		case TAND: printf("and				"); break;
		case TOR: printf("or				");  break;

		case TEQUAL: printf("equal to 				"); break;
		case TNOTEQU: printf("not equal to 				"); break;
		case TLESS: printf("less than				"); break;
		case TGREAT: printf("greater than 				"); break;
		case TLESSE: printf("less than or equal to 				"); break;
		case TGREATE: printf("greater than or eual to 				"); break;

		case TINC: printf("increase				"); break;
		case TDEC: printf("decrease				"); break;
		
		case TLEFTPAR: printf("left parenthesis				"); break;
		case TRIGHTPAR: printf("right parenthesis				"); break;
		case TLEFTBRACE: printf("left brace 				"); break;
		case TRIGHTBRACE: printf("right brace 				"); break;
		case TLEFTBRACKET: printf("left bracket				"); break;
		case TRIGHTBRACKET: printf("right bracket 					"); break;

		case TSEMI: printf("semicolon 				"); break;
		case TCOLON: printf("colon 				"); break;
	
		case TIDENT: printf("ident			%d			", currid); break;	  // Identifier�� ��� ST-index�� ���
		case TNUMBER: printf("number					"); break;
	}

	// Identifier Ȥ�� ������ ��� Token �� ���
	if (tn == TNUMBER)
		printf("%d \n", atoi(yytext)); 
	else
		printf("%s \n", yytext);
	
}


// main �Լ� 

void main()
{
	enum tokentypes tn;  // ��ū Ÿ��
	
	// ��� ��� (Line number, Token Type, ST-index, Token)
	printf("Line number	Token type	ST-index	Token\n");

	// ������ �ƴ� ���, Token ���� ���
	while ((tn = yylex()) != TEOF) {
		if (tn != TERROR)
			printtoken(tn);
	}

	// Error ���� Ƚ�� ���
	if (cErrors == 0)
		printf("No errors detected ");
	else printf("%d errors detected", cErrors);
	
	// �й� ���
	printf("\n\n[1983024 Choi mingyo, 2017007 Kim minseo, 2173109 Jeong Eunbi]\n");
}