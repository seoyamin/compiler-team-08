#define STsize 1000   // String Table(ST)의 size
#define HTsize 100		// Hash Table(HT)의 size
	
#define FALSE 0
#define TRUE 1

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;             // ST상에서 identifier의 인덱스
	HTpointer next;        // 다음 identifier를 위한 포인터
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

int nextid = 0;     // the current identifier 
int nextfree = 0;   // the next available index of ST

int hashcode;       //  identifier의 hashcode
int sameid;         //  identifier의 첫번째 인덱스

int found;          // for the previous occurrence of an identifier

int currid = 0;	// the current identifier 