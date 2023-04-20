#include <stdio.h>
#include <stdlib.h>
#include "glob.h"

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

	if (HT[hscode] != NULL) {  // hash code 위치에 어떠한 문자라도 존재하는 경우
		here = HT[hscode];
		while (here != NULL && found == FALSE) {  // 현재 가리키는 위치에 문자가 있고 아직 identifier가 발견되지 않은 경우
			found = TRUE;
			i = here->index;
			j = nid;
			sameid = i;

			while (ST[i] != '\0' && ST[j] != '\0' && found == TRUE) { // 문자를 하나씩 비교하면서 identifier의 일치 여부 판단
				if (ST[i] != ST[j])
					found = FALSE;
				else {
					i++;
					j++;
				}
			}
			here = here->next;  // linked list의 다음 identifier로 이동
		}
		currid = here->index;
	}
}


// ADDHT - Hash Table에 identifier를 추가하는 함수

void ADDHT(int hscode)
{
	HTpointer ptr;

	ptr = (HTpointer)malloc(sizeof(ptr));
	ptr->index = nextid;
	ptr->next = HT[hscode];
	HT[hscode] = ptr; // linked list로써 identifier 삽입
}



// CHECK - 케이스를 확인하고 적절한 함수를 호출하는 함수 

void check() {

	// [case 1] 더이상 읽을 문자가 없는 경우
	if (input == EOF && nextfree == nextid);

	// [case 2] 정상 or OVERLEN
	else if (err != illid && err != illid2) {
		if (nextfree == STsize) {
			err = overst;
			PrintError(err);
		}
		ST[nextfree++] = '\0';
		err = noerror;

		int len = nextfree - nextid - 1;  // 현재 읽어들인 identifier의 길이

		// [case 2-1] OVERLEN
		if (len > 10) {
			err = overlen;
			PrintError(err);
			nextfree = (nextid == 0) ? nextid : nextid - 1;
		}

		// [case 2-2] 정상
		else {
			ComputeHS(nextid, nextfree);
			LookupHS(nextid, hashcode);

			if (!found) { // Hash Table에 동일한 identifier가 존재하지 않는 경우
				printf("%6d			", nextid);
				for (i = nextid; i < nextfree - 1; i++)
					printf("%c", ST[i]);
				printf("		(entered)\n");
				ADDHT(hashcode);
			}
			else { // Hash Table에 동일한 identifier가 존재하는 경우
				printf("%6d			", sameid);
				for (i = nextid; i < nextfree - 1; i++)
					printf("%c", ST[i]);
				printf("		(already existed)\n");
				nextfree = nextid;
			}
		}
	}

	// [case 3] identifier가 숫자로 시작하는 경우
	else if (err == illid) {
		if (nextfree == STsize) {
			err = overst;
			PrintError(err);
		}

	}

	// [case 4] 허용되지 않는 identifier
	else {
		if (nextfree == STsize) {
			err = overst;
			PrintError(err);
		}
		PrintError(err);

		nextfree = nextid + 1;
		nextid = nextfree;
	}
}


// MAIN - main 

//We need to define
int STindex(const char* identifier)
{	

	//ST[nextfree++] = '\0'; 
	ComputeHS(nextid, nextfree);
	LookupHS(nextid, hashcode); 

	if (!found) { 
		ADDHT(hashcode); 
	}
	else {
		nextfree = nextid; 
	}
	
}