#define STsize 1000   // String Table(ST)�� size
#define HTsize 100		// Hash Table(HT)�� size
	
#define FALSE 0
#define TRUE 1

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;             // ST�󿡼� identifier�� �ε���
	HTpointer next;        // ���� identifier�� ���� ������
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

int nextid = 0;     // the current identifier 
int nextfree = 0;   // the next available index of ST

int hashcode;       //  identifier�� hashcode
int sameid;         //  identifier�� ù��° �ε���

int found;          // for the previous occurrence of an identifier

int currid = 0;	// the current identifier 