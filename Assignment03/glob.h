/*
* glob.h - 프로젝트의 전역 변수들
* progrmmer - 김민서, 정은비, 최민교
* date - 30/05/2023
*/
extern int currid;				// the current identifier 
extern unsigned int currlinenum;	// current line number
extern unsigned int cErrors;	// 등장한 에러 횟수

// enum idenerrortypes;				// identifier의 에러 타입 enum 

enum idenerrortypes {
	OVERLEN, ILLCHAR, ILLID
};

enum errortypes {
	NO_SEMI, NO_EXP, NO_NUM, NO_COLON, NO_DCL,
	NO_LEFTBRACKET, NO_RIGHTBRACKET, NO_LEFTBRACE, NO_RIGHTBRACE, NO_LEFTPAR, NO_RIGHTPAR,
	NO_EXP_OR_NO_SEMI, NO_NUM_OR_NO_RIGHTBRACKET, NO_RIGHT_SIDE_EXP
};

enum idenerrortypes identifier_et;

#define STsize 1000		// String Table(ST)의 size
#define HTsize 100		// Hash Table(HT)의 size

#define FALSE 0
#define TRUE 1

/* global variables from symtable.c */
typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;             // ST상에서 identifier의 인덱스
	HTpointer next;        // 다음 identifier를 위한 포인터
	char* type;	       // identifier의 속성
	int linenum;		// identifier의 linenum (We should add the initialization code for this variable.)
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

char identStr[20];
