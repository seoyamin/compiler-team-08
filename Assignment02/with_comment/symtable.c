/*
* symtable.c – 해시코드 계산함으로써 해당 identifier이 ST에 존재하는지 판단하고 ST에 identifier 삽입
* progrmmer – 김민서, 정은비, 최민교
* date - 04/26/2023
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "glob.h"

#define STsize 1000		// String Table(ST)의 size
#define HTsize 100		// Hash Table(HT)의 size

#define FALSE 0
#define TRUE 1

int nextid;				// the current identifier 
int nextfree;			// the next available index of ST

int hashcode;			//  identifier의 hashcode
int sameid;				//  identifier의 첫번째 인덱스

int found;				// for the previous occurrence of an identifier

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;             // ST상에서 identifier의 인덱스
	HTpointer next;        // 다음 identifier를 위한 포인터
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

nextid = 0;
nextfree = 0;
currid = 0;

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
			sameid = i;

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
	HT[hscode] = ptr;			// linked list로써 identifier 삽입
}


// SymbolTable - identifier를 이용하여 hashcode를 계산하고, 그 값을 Hash Table에 저장하는 함수
void SymbolTable(const char* identifier)
{	
	nextfree += strlen(identifier) + 1;

	for (int i = 0; i < strlen(identifier); i++) {
		ST[i+nextid] = identifier[i];
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
