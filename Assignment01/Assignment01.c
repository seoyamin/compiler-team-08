#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "testdata.txt"

#define STsize 1000 // size of string table
#define HTsize 100  // size of hash table

#define FALSE 0
#define TRUE 1

#define isLetter(x) ( ((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z') )]
#define isDigit(x) ( (x) >= '0' && (x) <= '9' )

typedef struct HTentry *HTpointer,
typedef struct HTentry {
	int index;       // index of identifier in ST
	HTpointer next;  // pointer to next identifier
} HTentry;


enum errorTypes { noerror, illsp, illid, overst };
typedef enum errorTypes ERRORtypes;

char seperators[] = ".,;:?!\t\n";

HTpointer HT[HTsize];
char ST[STsize];

int nextid = 0;    // the current identifier 
int nextfree = 0;  // the next available index of ST

int hashcode;      // hash code of identifier
int sameid;        // first index of identifier

int found;         // for the previous occurrence of an identifie

ERRORtypes err;

FILE* fp;          // to be a pointer to FILE
char input;


// Initialize - open input file

void initialize() {
	fp = fopen(FILE_NAME, "r");
	input = fget(fp);
}


// is Seperator - distinguish the seperator

int isSeperator(char c) {
	int i;
	int sep_len;

	sep_len = strlen(seperators);
	
	for (i = 0; i < sep_len; i++) {
		if (c == seperators[i])
			return 1;
	}

	return 0;
}


// print Heading

void PrintHeading() {
	printf("\n\n");
	printf("  -----------     ----------- \n");
	printf("  Index in ST      identifier  \n");
	printf("  -----------     ----------- \n");
	printf("\n");
}


// Print HS table

void PrintHStable() {
	int i, j;
	HTpointer here;

	printf("\n\n\n\n\n [[ HASH TABLE ]] \n\n");

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
		printf("\n\n\n < %5d characters are used in the string table > \n", nextfree);
	}
}


// PrintError

void PrintError(ERRORtypes err){
	switch(err){
		case overst :
			printf("...Error...		OVERFLOW");
			PrintHStable();
			exit(0);
			break;
		case illsp :
			printf("...Error...		%c is illegal seperator \n", input);
			break;
		case illid :
			printf("...Error... ");
			while(input != EOF && (isLetter(input) || isDigit(input))){
				printf("%c", input);
				input = fgetc(fp);
			}
			printf("start with digit \n");
			break;
	}
}


// Skip Seperators

void SkipSeperators(){
	while(input != EOF && !(isLetter(input) || isDigit(input))){
		if(!isSeperator(input)){
			err = illsp;
			PrintError(err);
		}
		input = fgetc(fp);
	}
}


// ReadIO

void ReadID() {
	nextid = nextfree;
	if(isDigit(input)){
		err = illid;
		PrintError(err);
	}
	else{
		while(input != EOF && (isLetter(input) || isDigit(input))){
			if(nextfree == STsize){
				err = overst;
				PrintError(err);
			}
			ST[nextfree++] = input;
			input = fgetc(fp);
		}
	}
}


// ComputeHS

void ComputeHS(int nid, int nfree){
	int code int i;
	code = 0;
	
	for(i = nid; i < nfree - 1; i++)
		code += (int)ST[i];
	hashcode = code % HTsize;
}
