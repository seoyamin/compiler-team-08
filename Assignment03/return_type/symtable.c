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

int currid = 0;				// the current index of ST
int nextid = 0;				// the next index of ST 
int nextfree = 0;			// the next available index of ST

int hashcode;				// identifier의 hashcode
int sameid;					// identifier의 첫번째 인덱스

int found;					// for the previous occurrence of an identifier

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
	HTpointer ptr;

	ptr = (HTpointer)malloc(sizeof(ptr));
	currid = nextid;
	ptr->index = nextid;
	ptr->next = HT[hscode];
	ptr->type = NULL;
	ptr->returntype = NULL;
	ptr->linenum = currlinenum;		// identifier가 선언된 위치의 line number 
	HT[hscode] = ptr;				// linked list로써 identifier 삽입
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
				while (ST[j] != '\0' && j < STsize)
					printf("%c", ST[j++]);

				printf("\t(%s, line %d)\n", here->type, here->linenum);

				//if(here->returntype == NULL) printf("\t(%s, line %d)\n", here->type, here->linenum);
				//else printf("\t(%s, %s, line %d)\n", here->type, here->returntype, here->linenum);
				
				//printf("\t(%s, ", here->type);
				//printf("return type = %s, ", here->returntype);
				//printf("line %d)\n", here->linenum);
			}
			printf("\n");
		}
	}
}

// SymbolTable - identifier를 이용하여 hashcode를 계산하고, 그 값을 Hash Table에 저장하는 함수
void SymbolTable(const char* identifier)
{
	strcpy(identStr, identifier);

	nextfree += strlen(identifier) + 1;

	for (int i = 0; i < strlen(identifier); i++) {	// String Table에 identifier 삽입
		ST[i + nextid] = identifier[i];
	}
	ST[nextfree] = '\0';

	currid = nextid;

	ComputeHS(nextid, nextfree);	// hash code 계산
	LookupHS(nextid, hashcode);		// Hash Table에 현재 identifier가 저장되어 있는지 판단

	if (!found) {					// [case 1] HT에 저장된 이력 없는 경우 - 저장
		ADDHT(hashcode);
	}
	else {							// [case 2] HT에 저장된 이력 있는 경우
		nextfree = nextid;
	}
	nextid = nextfree;
}
