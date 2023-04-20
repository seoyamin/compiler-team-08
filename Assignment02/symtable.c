#include <stdio.h>
#include <stdlib.h>
#include "glob.h"

// ComputeHS - �ؽ��ڵ� ����ϴ� �Լ�
void ComputeHS(int nid, int nfree) {
	int code = 0;
	int i;

	for (i = nid; i < nfree - 1; i++)
		code += (int)ST[i];
	hashcode = code % HTsize;
}


// LookupHs - Hash Table�� ���� ���� �о���� identifier�� ���� ���θ� �Ǵ��ϴ� �Լ�
void LookupHS(int nid, int hscode)
{
	HTpointer here;
	int i, j;
	found = FALSE; 	// Hash Table�� �б� ��, FALSE�� �ʱ�ȭ

	if (HT[hscode] != NULL) {  // hash code ��ġ�� ��� ���ڶ� �����ϴ� ���
		here = HT[hscode];
		while (here != NULL && found == FALSE) {  // ���� ����Ű�� ��ġ�� ���ڰ� �ְ� ���� identifier�� �߰ߵ��� ���� ���
			found = TRUE;
			i = here->index;
			j = nid;
			sameid = i;

			while (ST[i] != '\0' && ST[j] != '\0' && found == TRUE) { // ���ڸ� �ϳ��� ���ϸ鼭 identifier�� ��ġ ���� �Ǵ�
				if (ST[i] != ST[j])
					found = FALSE;
				else {
					i++;
					j++;
				}
			}
			here = here->next;  // linked list�� ���� identifier�� �̵�
		}
		currid = here->index;
	}
}


// ADDHT - Hash Table�� identifier�� �߰��ϴ� �Լ�

void ADDHT(int hscode)
{
	HTpointer ptr;

	ptr = (HTpointer)malloc(sizeof(ptr));
	ptr->index = nextid;
	ptr->next = HT[hscode];
	HT[hscode] = ptr; // linked list�ν� identifier ����
}



// CHECK - ���̽��� Ȯ���ϰ� ������ �Լ��� ȣ���ϴ� �Լ� 

void check() {

	// [case 1] ���̻� ���� ���ڰ� ���� ���
	if (input == EOF && nextfree == nextid);

	// [case 2] ���� or OVERLEN
	else if (err != illid && err != illid2) {
		if (nextfree == STsize) {
			err = overst;
			PrintError(err);
		}
		ST[nextfree++] = '\0';
		err = noerror;

		int len = nextfree - nextid - 1;  // ���� �о���� identifier�� ����

		// [case 2-1] OVERLEN
		if (len > 10) {
			err = overlen;
			PrintError(err);
			nextfree = (nextid == 0) ? nextid : nextid - 1;
		}

		// [case 2-2] ����
		else {
			ComputeHS(nextid, nextfree);
			LookupHS(nextid, hashcode);

			if (!found) { // Hash Table�� ������ identifier�� �������� �ʴ� ���
				printf("%6d			", nextid);
				for (i = nextid; i < nextfree - 1; i++)
					printf("%c", ST[i]);
				printf("		(entered)\n");
				ADDHT(hashcode);
			}
			else { // Hash Table�� ������ identifier�� �����ϴ� ���
				printf("%6d			", sameid);
				for (i = nextid; i < nextfree - 1; i++)
					printf("%c", ST[i]);
				printf("		(already existed)\n");
				nextfree = nextid;
			}
		}
	}

	// [case 3] identifier�� ���ڷ� �����ϴ� ���
	else if (err == illid) {
		if (nextfree == STsize) {
			err = overst;
			PrintError(err);
		}

	}

	// [case 4] ������ �ʴ� identifier
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