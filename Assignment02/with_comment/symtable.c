#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "glob.h"

#define STsize 1000		// String Table(ST)�� size
#define HTsize 100		// Hash Table(HT)�� size

#define FALSE 0
#define TRUE 1

int nextid;				// the current identifier 
int nextfree;			// the next available index of ST

int hashcode;			//  identifier�� hashcode
int sameid;				//  identifier�� ù��° �ε���

int found;				// for the previous occurrence of an identifier

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;             // ST�󿡼� identifier�� �ε���
	HTpointer next;        // ���� identifier�� ���� ������
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

nextid = 0;
nextfree = 0;
currid = 0;

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

	// hash code ��ġ�� ��� ���ڶ� �����ϴ� ���
	if (HT[hscode] != NULL) {  
		here = HT[hscode];
		while (here != NULL && found == FALSE) {		// ���� ����Ű�� ��ġ�� ���ڰ� �ְ� ���� identifier�� �߰ߵ��� ���� ���
			found = TRUE;
			i = here->index;
			j = nid;
			sameid = i;

			while (ST[i] != '\0' && found == TRUE) {	// ���ڸ� �ϳ��� ���ϸ鼭 identifier�� ��ġ ���� �Ǵ�

				// identifier�� ��ġ���� �ʴ� ������ ���
				if (ST[i] != ST[j])		
					found = FALSE;

				// identifier�� ��ġ�ϴ� ������ ���
				else {					
					i++;
					j++;
					currid = here->index;
				}
			}
			here = here->next;  // linked list�� ���� identifier�� �̵�
		}
	}
}


// ADDHT - Hash Table�� identifier�� �߰��ϴ� �Լ�

void ADDHT(int hscode)
{
	HTpointer ptr;

	ptr = (HTpointer)malloc(sizeof(ptr));
	ptr->index = nextid;
	ptr->next = HT[hscode];
	HT[hscode] = ptr;			// linked list�ν� identifier ����
}


// SymbolTable - identifier�� �̿��Ͽ� hashcode�� ����ϰ�, �� ���� Hash Table�� �����ϴ� �Լ�
void SymbolTable(const char* identifier)
{	
	nextfree += strlen(identifier) + 1;

	for (int i = 0; i < strlen(identifier); i++) {
		ST[i+nextid] = identifier[i];
	}
	ST[nextfree] = '\0';
	
	currid = nextid;
	
	ComputeHS(nextid, nextfree);	// hash code ���
	LookupHS(nextid, hashcode);		// Hash Table�� ���� identifier�� ����Ǿ� �ִ��� �Ǵ�
	
	if (!found) {					// [case 1] HT�� ����� �̷� ���� ��� - ����
		ADDHT(hashcode); 
	}
	else {							// [case 2] HT�� ����� �̷� �ִ� ���
		nextfree = nextid; 
	}
	nextid = nextfree;
}