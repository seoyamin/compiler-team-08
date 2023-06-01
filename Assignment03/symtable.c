/*
* symtable.c - 해시코드 계산함으로써 해당 identifier이 ST에 존재하는지 판단하고 ST에 identifier 삽입
* programmer - 김민서, 정은비, 최민교
* date - 05/30/2023
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "glob.h"

extern yytext;

int currid = 0;				// String Table에서 현재 index
int nextid = 0;				// String Table에서 다음 index
int nextfree = 0;			// String Table에서 다음 identifier가 저장될 위치의 index
int hashcode;				// identifier의 hashcode
int found;					// identifier의 존재 여부

// ComputeHS - 해시코드 계산하는 함수
void ComputeHS(int nid, int nfree) {
	int code = 0;
	int i;

	for (i = nid; i < nfree - 1; i++)
		code += (int)ST[i];
	hashcode = code % HTsize;
}


// LookupHs - Hash Table을 보고 현재 읽어들인 identifier의 존재 여부를 판단하는 함수
void LookupHS(int nid, int hscode)
{
	HTpointer here;
	int i, j;
	found = FALSE; 	// Hash Table을 읽기 전, FALSE로 초기화

	// hash code 위치에 어떠한 문자라도 존재하는 경우
	if (HT[hscode] != NULL) {
		here = HT[hscode];
		while (here != NULL && found == FALSE) {		// 현재 가리키는 위치에 문자가 있고 아직 identifier가 발견되지 않은 경우
			found = TRUE;
			i = here->index;
			j = nid;

			while (ST[i] != '\0' && found == TRUE) {	// 문자를 하나씩 비교하면서 identifier의 일치 여부 판단

				// identifier와 일치하지 않는 문자인 경우
				if (ST[i] != ST[j])
					found = FALSE;

				// identifier와 일치하는 문자인 경우
				else {
					i++;
					j++;
					currid = here->index;
				}
			}
			here = here->next;  // linked list의 다음 identifier로 이동
		}
	}
}


// ADDHT - Hash Table에 identifier를 추가하는 함수
void ADDHT(int hscode)
{
	HTpointer ptr = (HTpointer)malloc(sizeof(HTentry));
	if (ptr != NULL) {
		currid = nextid;
		ptr->index = nextid;			// identifier의 index
		ptr->linenum = currlinenum;		// identifier가 선언된 위치의 line number 
		ptr->next = HT[hscode];			// identifier의 next
		ptr->returntype = -1;			// identifier의 return type (identfier가 함수명인 경우: void->0, int->1)
		ptr->type = NULL;			// identifier의 type
		HT[hscode] = ptr;				// linked list로써 identifier 삽입
	}
}

// PrintHSTable - Hash Table에 저장된 identifier를 출력하는 함수
void PrintHStable() {
	int i, j;
	HTpointer here;

	// Heading 출력
	printf("\n\n [ HASH TABLE ] \n\n");

	// Hash Table 출력
	for (i = 0; i < HTsize; i++) {
		if (HT[i] != NULL) {
			printf(" Hash Code %3d \n", i);
			for (here = HT[i]; here != NULL; here = here->next) {
				j = here->index;
				printf(" -  ");
				while (ST[j] != '\0')	// String Table에 저장되어있는 identifier의 문자를 하나씩 출력
					printf("%c", ST[j++]);
				if (here->type == "function name") {	// identifier의 type이 함수명인 경우
					if (here->returntype == RETURN_VOID)
						printf("\t(%s, line %d, return void)\n", here->type, here->linenum);
					else if (here->returntype == RETURN_INT)
						printf("\t(%s, line %d, return int)\n", here->type, here->linenum);
				}
				else	// identfier의 type이 함수명이 아닌 경우
					printf("\t(%s, line %d)\n", here->type, here->linenum);
			}
			printf("\n");
		}
	}
}

// SymbolTable - identifier의 hashcode를 계산하고, 그 값을 Hash Table에 저장하는 함수
void SymbolTable(const char* identifier)
{
	strcpy(identStr, identifier);	// identifier 문자열을 identStr에 복사

	nextfree += strlen(identifier) + 1;	// nextfree 계산

	for (int i = 0; i < strlen(identifier); i++) {	// String Table에 identifier 삽입
		ST[i + nextid] = identifier[i];
	}
	ST[nextfree] = '\0';

	currid = nextid;

	ComputeHS(nextid, nextfree);	// hash code 계산
	LookupHS(nextid, hashcode);		// Hash Table에 현재 identifier가 저장되어 있는지 판단

	if (!found) {					// [case 1] HT에 저장된 이력 없는 경우 - 저장
		ADDHT(hashcode);
		currid = nextid;
	}
	else {							// [case 2] HT에 저장된 이력 있는 경우
		for (int i = 0; i < strlen(identifier); i++) {	// ST에 저장한 내용 삭제
			ST[nextfree - i-1] = NULL;
		}
		nextfree = nextid;
	}
	nextid = nextfree;
}
