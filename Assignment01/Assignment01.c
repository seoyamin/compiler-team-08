#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

#define FILE_NAME "testdata.txt"

#define STsize 1000   // String Table(ST)의 size
#define HTsize 100    // Hash Table(HT)의 size

#define FALSE 0
#define TRUE 1

#define isLetter(x) ( ((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z') || x == '_' )
#define isDigit(x) ( (x) >= '0' && (x) <= '9')

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;             // ST상에서 identifier의 인덱스
	HTpointer next;        // 다음 identifier를 위한 포인터
} HTentry;


enum errorTypes { noerror, illsp, illid, overst, overlen, illid2 };   //  발생 가능한 에러 타입 (illid2는 not allowed 문자 에러를 의미)
typedef enum errorTypes ERRORtypes;

char seperators[] = ".,;:?!\t \n";

HTpointer HT[HTsize];
char ST[STsize];

int nextid = 0;     // the current identifier 
int nextfree = 0;   // the next available index of ST

int illid2var = 0;  // err = illid2 인 경우 잘못된 문자가 등장한 인덱스 

int hashcode;       //  identifier의 hashcode
int sameid;         //  identifier의 첫번째 인덱스

int found;          // for the previous occurrence of an identifier

ERRORtypes err;     // 발생한 에러 타입

FILE* fp;           // FILE의 포인터
char input;

int i;


// Initialize - input 파일을 여는 함수

void initialize() {
	fp = fopen(FILE_NAME, "r");
	input = fgetc(fp);
}


// is Seperator - seperator인지 여부를 판단하는 함수 

int isSeperator(char c) {
	int i;
	int sep_len;

	sep_len = strlen(seperators);

	for (i = 0; i < sep_len; i++) {
		if (c == seperators[i])
			return 1;			// Seperator인 경우
	}

	return 0;    //  Seperator가 아닌 경우
}


// PrintHeading - Heading(Index in ST, identifier)을 출력하는 함수

void PrintHeading() {
	printf("\n\n");
	printf("  -----------     ----------- \n");
	printf("  Index in ST      identifier  \n");
	printf("  -----------     ----------- \n");
	printf("\n");
}


// PrintHStable - Hash Table을 출력하는 함수

void PrintHStable() {
	int i, j;
	HTpointer here;

	// Heading 출력
	printf("\n\n\n\n\n [[ HASH TABLE ]] \n\n");

	// Hash Table 출력
	for (i = 0; i < HTsize; i++) {
		if (HT[i] != NULL) {
			printf(" Hash Code %3d : ", i);
			for (here = HT[i]; here != NULL; here = here->next) {
				j = here->index;
				while (ST[j] != '\0' && j < STsize)
					printf("%c", ST[j++]);
				printf("    ");
			}
			printf("\n");
		}

	}
	printf("\n\n\n < %5d characters are used in the string table > \n", nextfree);   // ST상의 characters 수 출력
	printf("\n\n1983024 최민교, 2017007 김민서, 2173109 정은비\n");
}


// PrintError

void PrintError(ERRORtypes err) {
	switch (err) {
	case overst:
		printf("...Error...		OVERFLOW");
		PrintHStable();
		exit(0);
		break;

	case illsp:
		printf("...Error...		%c is illegal seperator \n", input);
		break;

	case illid:
		printf("...Error...		");
		while (input != EOF && (isLetter(input) || isDigit(input))) {
			printf("%c", input);
			input = fgetc(fp);
		}
		printf("		start with digit \n");
		break;

	case overlen:
		printf("...Error...		");
		for (i = nextid; i < nextfree; i++) {
			printf("%c", ST[i]);
		}
		printf("		too long identifier \n");
		break;

	case illid2:
		printf("...Error...		");
		for (i = nextid; i < nextfree; i++) {
			printf("%c", ST[i]);
			if (!(isLetter(ST[i]) || isDigit(ST[i]))) illid2var = i;
		}

		printf("	%c is not allowed\n", ST[illid2var]);
		break;
	}
}


// Skip Seperators

void SkipSeperators() {
	while (input != EOF && !(isLetter(input) || isDigit(input))) {
		if (!isSeperator(input)) {
			err = illsp;
			PrintError(err);
		}
		input = fgetc(fp);
	}
}


// ReadIO

void ReadID() {
	nextid = nextfree;
	if (isDigit(input)) {
		err = illid;
		PrintError(err);
	}
	else {
		// while (input != EOF && (isLetter(input) || isDigit(input))) {
		while (input != EOF && !isSeperator(input)) {
			if (nextfree == STsize) {
				err = overst;
				PrintError(err);
			}
			if (!(isLetter(input) || isDigit(input)))
				err = illid2;
			ST[nextfree++] = input;
			input = fgetc(fp);
		}
	}
}


// ComputeHS

void ComputeHS(int nid, int nfree) {
	int code = 0;
	int i;

	for (i = nid; i < nfree - 1; i++)
		code += (int)ST[i];
	hashcode = code % HTsize;
}

// LookupHs

void LookupHS(int nid, int hscode)
{
	HTpointer here;
	int i, j;

	found = FALSE;
	if (HT[hscode] != NULL) {
		here = HT[hscode];
		while (here != NULL && found == FALSE) {
			found = TRUE;
			i = here->index;
			j = nid;
			sameid = i;

			while (ST[i] != '\0' && ST[j] != '\0' && found == TRUE) {
				if (ST[i] != ST[j])
					found = FALSE;
				else {
					i++;
					j++;
				}
			}
			here = here->next;
		}
	}
}

// ADDHT

void ADDHT(int hscode)
{
	HTpointer ptr;

	ptr = (HTpointer)malloc(sizeof(ptr));
	ptr->index = nextid;
	ptr->next = HT[hscode];
	HT[hscode] = ptr;
}


// CHECK
void check() {

	// [case 1] 정상 or OVERLEN
	if (input == EOF && nextfree == nextid);

	else if (err != illid && err != illid2) {
		if (nextfree == STsize) {
			err = overst;
			PrintError(err);
		}
		ST[nextfree++] = '\0';
		err = noerror;

		int len = nextfree - nextid - 1;

		// [case 1-1] OVERLEN
		if (len > 10) {
			err = overlen;
			PrintError(err);
			nextfree = (nextid == 0) ? nextid : nextid - 1;
		}

		// [case 1-2] 정상
		else {
			ComputeHS(nextid, nextfree);
			LookupHS(nextid, hashcode);

			if (!found) {
				printf("%6d			", nextid);
				for (i = nextid; i < nextfree - 1; i++)
					printf("%c", ST[i]);
				printf("		(entered)\n");
				ADDHT(hashcode);
			}
			else {
				printf("%6d			", sameid);
				for (i = nextid; i < nextfree - 1; i++)
					printf("%c", ST[i]);
				printf("		(already existed)\n");
				nextfree = nextid;
			}
		}
	}

	// [case 2] ILLID : start with digit
	else if (err == illid) {
		if (nextfree == STsize) {
			err = overst;
			PrintError(err);
		}
		// PrintError(err);

	}

	// [case 3] ILLID2 : NOT ALLOWED IDENTIFIER
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


// MAIN

int main()
{
	PrintHeading();
	initialize();

	while (input != EOF) {
		err = noerror;
		SkipSeperators();
		ReadID();

		if (input == EOF) {
			check();
			break;
		}

		check();
	}

	PrintHStable();
}