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
extern int cReturntype;			// EB: 현재 return type 

enum tokenerrortypes identifier_et;
enum tokenerrortypes {
	OVERLEN, ILLCHAR, ILLID
};

enum parseerrortypes {
	NO_SEMI, NO_EXP, NO_NUM, NO_COLON, NO_DCL,
	NO_LEFTBRACKET, NO_RIGHTBRACKET, NO_LEFTBRACE, NO_RIGHTBRACE, NO_LEFTPAR, NO_RIGHTPAR,
	NO_EXP_OR_NO_SEMI, NO_NUM_OR_NO_RIGHTBRACKET, NO_RIGHT_SIDE_EXP, NOT_DEFINED_SPECIFIER
};

enum funcreturntypes {
	RETURN_VOID, RETURN_INT
};

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	unsigned int index;             // ST상에서 identifier의 인덱스
	HTpointer next;					// 다음 identifier를 위한 포인터
	char* type;						// identifier의 속성
	unsigned int linenum;			// identifier가 선언된 위치의 line number 
	int returntype;		// EB: identifier의 return type (identfier가 함수명인 경우)
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

char identStr[20];
