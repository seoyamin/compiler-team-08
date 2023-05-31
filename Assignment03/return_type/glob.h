/*
* glob.h - 프로젝트의 전역 변수들
* progrmmer - 김민서, 정은비, 최민교
* date - 30/05/2023
*/

#define STsize 1000		// String Table(ST)의 size
#define HTsize 100		// Hash Table(HT)의 size

#define FALSE 0
#define TRUE 1

extern unsigned int currlinenum;	// 현재 line number
extern unsigned int cErrors;		// 등장한 에러 횟수

// Scanner에서 발생 가능한 에러 타입
enum tokenerrortypes identifier_et;
enum tokenerrortypes {
	OVERLEN, ILLCHAR, ILLID
};

// Parser에서 발생 가능한 에러 타입
enum parseerrortypes {
	NO_SEMI, NO_EXP, NO_NUM, NO_COLON, NO_DCL,
	NO_LEFTBRACKET, NO_RIGHTBRACKET, NO_LEFTBRACE, NO_RIGHTBRACE, NO_LEFTPAR, NO_RIGHTPAR,
	NO_EXP_OR_NO_SEMI, NO_NUM_OR_NO_RIGHTBRACKET, NO_RIGHT_SIDE_EXP
};

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;             // ST상에서 identifier의 인덱스
	HTpointer next;        // 다음 identifier를 위한 포인터
	char* type;	       		// identifier의 속성
	char* returntype;	    // 함수 identifier의 리턴 타입
	int linenum;			// identifier의 linenum (We should add the initialization code for this variable.)
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

char identStr[20];			// 현재 identifier를 저장하는 변수
char identReturnType[20];	// 현재 함수 identifier의 리턴 타입을 저장하는 변수
