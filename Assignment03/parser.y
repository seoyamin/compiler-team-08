%{
/*
* parser.y - Modified MiniC를 위한 Syntax Analyzer
* programmer - 김민서, 정은비, 최민교
* date - 05/30/2023
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "tn.h"
#include "glob.h"

extern yyerror();
extern printParseError();

void defineReturnType(int returntype, const char *identifier); 
void defineIdentType(const char *type, const char *identifier); 
%}

%nonassoc TLOWERTHANELSE
%nonassoc TELSE

%token TEOF, TCONST, TELSE, TIF, TINT, TRETURN
%token TVOID, TWHILE, TADD, TSUB, TMUL
%token TDIV, TMOD, TASSIGN, TADDASSIGN, TSUBASSIGN
%token TMULASSIGN, TDIVASSIGN, TMODASSIGN, TNOT, TAND
%token TOR, TEQUAL, TNOTEQU, TLESS, TGREAT
%token TLESSE, TGREATE, TINC, TDEC, TLEFTPAR
%token TRIGHTPAR, TCOLON, TLEFTBRACE, TRIGHTBRACE, TLEFTBRACKET
%token TRIGHTBRACKET, TSEMI, TNUMBER, TIDENT, TERROR

%%
mini_c 				: translation_unit										
					;
translation_unit 	: external_dcl											
					| translation_unit external_dcl						
					;
external_dcl 		: function_def											
					| declaration											
					;
function_def 		: function_header compound_st	
					| function_header compound_st	error					{yyerrok; printParseError("function_def", INVALID_LOCATION);}
					;
function_header 	: dcl_spec function_name formal_param					
					;
dcl_spec 			: dcl_specifiers										
					;
dcl_specifiers 		: dcl_specifier											
					| dcl_specifiers dcl_specifier						
					;
dcl_specifier 		: type_qualifier										
					| type_specifier										
					;
type_qualifier 		: TCONST											
					;
type_specifier 		: TINT													{cReturntype = 1;}
		 			| TVOID													{cReturntype = 0;}
					;
function_name 		: TIDENT												{defineIdentType("function name", identStr); defineReturnType(cReturntype, identStr);}
					| TERROR	
					;											
formal_param 		: TLEFTPAR TRIGHTPAR 									
					| TLEFTPAR error										{yyerrok; printParseError("formal_param", NO_RIGHTPAR);}
					| TLEFTPAR formal_param_list TRIGHTPAR					
					| TLEFTPAR formal_param_list error						{yyerrok; printParseError("formal_param", NO_RIGHTPAR);}
					;
formal_param_list 	: param_dcl												
		    		| formal_param_list TCOLON param_dcl 					
					| formal_param_list param_dcl error	 					{yyerrok; printParseError("formal_param_list", NO_COLON);}
					;
param_dcl 			: dcl_spec declarator									
					| dcl_spec error										{yyerrok; printParseError("param_dcl", NO_DCL);}
					;
compound_st 		: TLEFTBRACE TRIGHTBRACE								
					| TLEFTBRACE error										{yyerrok; printParseError("compound_st", NO_RIGHTBRACE);}
					| TLEFTBRACE declaration_list TRIGHTBRACE			
					| TLEFTBRACE declaration_list error		 				{yyerrok; printParseError("compound_st", NO_RIGHTBRACE);}
					| TLEFTBRACE statement_list TRIGHTBRACE					
					| TLEFTBRACE statement_list error						{yyerrok; printParseError("compound_st", NO_RIGHTBRACE);}
					| TLEFTBRACE declaration_list statement_list TRIGHTBRACE	
					| TLEFTBRACE declaration_list statement_list error		{yyerrok; printParseError("compound_st", NO_RIGHTBRACE);}
					;
declaration_list 	: declaration										
					| declaration_list declaration 						
					;
declaration 		: dcl_spec init_dcl_list TSEMI							
					| dcl_spec init_dcl_list error 							{yyerrok; printParseError("declaration", NO_SEMI);}
					;
init_dcl_list 		: init_declarator										
					| init_dcl_list TCOLON init_declarator					
					| init_dcl_list init_declarator error			     		{yyerrok; printParseError("init_dcl_list", NO_COLON);}
					;
init_declarator		: declarator											
					| declarator TASSIGN TNUMBER							
					| declarator TASSIGN error								{yyerrok; printParseError("init_declarator", NO_NUM);}
					;
declarator 			: TIDENT												{defineIdentType("integer scalar variable", identStr);}
					| TERROR											
					| TIDENT TLEFTBRACKET TNUMBER TRIGHTBRACKET				{defineIdentType("integer array variable", identStr);}
					| TERROR TLEFTBRACKET TNUMBER TRIGHTBRACKET				
					| TIDENT TLEFTBRACKET TRIGHTBRACKET						{defineIdentType("integer array variable", identStr);}
					| TERROR TLEFTBRACKET TRIGHTBRACKET			
					| TIDENT TLEFTBRACKET TNUMBER error						{yyerrok; printParseError("declarator", NO_RIGHTBRACKET);}
					| TIDENT TLEFTBRACKET TIDENT TRIGHTBRACKET			    {printParseError("declarator", NO_NUM);}
					| TIDENT TLEFTBRACKET error								{yyerrok; printParseError("declarator", NO_NUM_OR_NO_RIGHTBRACKET);}
					;
statement_list 		: statement												
		 			| statement_list statement 							
					;
statement 			: compound_st											
	   				| expression_st											
	   				| if_st													
	   				| while_st												
	   				| return_st												
					;
expression_st 		: TSEMI													
					| expression TSEMI										
					| expression error										{yyerrok; printParseError("expression_st", NO_SEMI);}
					;
if_st 				: TIF TLEFTPAR expression TRIGHTPAR statement %prec TLOWERTHANELSE
					| TIF TLEFTPAR expression TRIGHTPAR statement TELSE statement 
					| TIF TLEFTPAR expression error statement				{yyerrok; printParseError("if_st", NO_RIGHTBRACKET);}
					| TIF TLEFTPAR TRIGHTPAR								{yyerrok; printParseError("if_st", NO_EXP);}
					| TIF error	expression TRIGHTPAR statement				{yyerrok; printParseError("if_st", NO_LEFTPAR);}
					; 
while_st 			: TWHILE TLEFTPAR expression TRIGHTPAR statement 		
					| TWHILE TLEFTPAR expression error statement			{yyerrok; printParseError("while_st", NO_RIGHTPAR);}
					| TWHILE error expression TRIGHTPAR statement			{yyerrok; printParseError("while_st", NO_LEFTPAR);}
					;
return_st 			: TRETURN TSEMI											// return;의 경우	
					| TRETURN expression TSEMI								// return 3;의 경우
					| TRETURN expression error								{yyerrok; printParseError("return_st", NO_SEMI);}  // return 3의 경우 -> no semi
					| TRETURN error											{yyerrok; printParseError("return_st", NO_EXP_OR_NO_SEMI);}  // return 의 경우 -> no return value + no semi
					;
expression 			: assignment_exp										
assignment_exp 		: logical_or_exp										
					| unary_exp TASSIGN assignment_exp 				
					| unary_exp TADDASSIGN assignment_exp					
					| unary_exp TSUBASSIGN assignment_exp					
					| unary_exp TMULASSIGN assignment_exp				
					| unary_exp TDIVASSIGN assignment_exp				 	
					| unary_exp TMODASSIGN assignment_exp				 
					| unary_exp TASSIGN error								{yyerrok; printParseError("assignment_exp_assign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TADDASSIGN error							{yyerrok; printParseError("assignment_exp_addassign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TSUBASSIGN error							{yyerrok; printParseError("assignment_exp_subassign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TMULASSIGN error							{yyerrok; printParseError("assignment_exp_mulassign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TDIVASSIGN error			 				{yyerrok; printParseError("assignment_exp_divassign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TMODASSIGN error							{yyerrok; printParseError("assignment_exp_modassign", NO_RIGHT_SIDE_EXP);}
					;
logical_or_exp 		: logical_and_exp									
					| logical_or_exp TOR logical_and_exp 				
					| logical_or_exp TOR error								{yyerrok; printParseError("logical_or_exp", NO_RIGHT_SIDE_EXP);}
					;
logical_and_exp 	: equality_exp											
		 			| logical_and_exp TAND equality_exp 				
					| logical_and_exp TAND error							{yyerrok; printParseError("logical_and_exp", NO_RIGHT_SIDE_EXP);}
					;
equality_exp 		: relational_exp										
					| equality_exp TEQUAL relational_exp 				
					| equality_exp TNOTEQU relational_exp 					
					| equality_exp TEQUAL error								{yyerrok; printParseError("equality_exp_equal", NO_RIGHT_SIDE_EXP);}
					| equality_exp TNOTEQU error							{yyerrok; printParseError("equality_exp_notequal", NO_RIGHT_SIDE_EXP);}
					;
relational_exp 		: additive_exp 										
					| relational_exp TGREAT additive_exp	 				
					| relational_exp TLESS additive_exp				
					| relational_exp TGREATE additive_exp				
					| relational_exp TLESSE additive_exp				
					| relational_exp TGREAT	error				 			{yyerrok; printParseError("relational_exp_great", NO_RIGHT_SIDE_EXP);}
					| relational_exp TLESS error							{yyerrok; printParseError("relational_exp_less", NO_RIGHT_SIDE_EXP);}
					| relational_exp TGREATE error							{yyerrok; printParseError("relational_exp_greate", NO_RIGHT_SIDE_EXP);}
					| relational_exp TLESSE error							{yyerrok; printParseError("relational_exp_lesse", NO_RIGHT_SIDE_EXP);}
					;
additive_exp 		: multiplicative_exp								
					| additive_exp TADD multiplicative_exp				
					| additive_exp TSUB multiplicative_exp 				
					| additive_exp TADD error								{yyerrok; printParseError("additive_exp_add", NO_RIGHT_SIDE_EXP);}
					| additive_exp TSUB error					 			{yyerrok; printParseError("additive_exp_sub", NO_RIGHT_SIDE_EXP);}
					;
multiplicative_exp 	: unary_exp												
		    		| multiplicative_exp TMUL unary_exp	 			
					| multiplicative_exp TDIV unary_exp					
				 	| multiplicative_exp TMOD unary_exp					
					| multiplicative_exp TMUL error			 				{yyerrok; printParseError("multiplicative_exp_mul", NO_RIGHT_SIDE_EXP);}
					| multiplicative_exp TDIV error							{yyerrok; printParseError("multiplicative_exp_div", NO_RIGHT_SIDE_EXP);}
				 	| multiplicative_exp TMOD error							{yyerrok; printParseError("multiplicative_exp_mod", NO_RIGHT_SIDE_EXP);}
					;
unary_exp 			: postfix_exp											
	   				| TSUB unary_exp										
	   				| TNOT unary_exp										
	   				| TINC unary_exp								
	   				| TDEC unary_exp										
					;
postfix_exp 		: primary_exp											
	      			| postfix_exp TLEFTBRACKET expression TRIGHTBRACKET 	
					| postfix_exp TLEFTBRACKET expression error				{yyerrok; printParseError("postfix_exp", NO_RIGHTBRACKET);}
					| postfix_exp TLEFTPAR opt_actual_param TRIGHTPAR 		
					| postfix_exp TLEFTPAR opt_actual_param error			{yyerrok; printParseError("postfix_exp", NO_RIGHTBRACKET);}
	      			| postfix_exp TINC									
	      			| postfix_exp TDEC									
					;
opt_actual_param 	: actual_param									
		  			|														
					;
actual_param 		: actual_param_list										
					;
actual_param_list 	: assignment_exp								
					| actual_param_list TCOLON assignment_exp 				
					;
primary_exp 		: TIDENT											
					| TERROR												
					| TNUMBER												
	     			| TLEFTPAR expression TRIGHTPAR				
					| TLEFTPAR expression error								{yyerrok; printParseError("primary_exp", NO_RIGHTPAR);}
					;
%%

// defineReturnType - function name에 해당하는 identifier의 return type을 저장하는 함수
void defineReturnType(int returntype, const char *identifier)
{
	int length = strlen(identifier);
	int found = FALSE;

	// 해당 identifier의 해시코드 구하기
	int hashcode = 0;		
	for (int i = 0; i < length; i++) {
		hashcode += (int)identifier[i];
	}
	hashcode %= HTsize;

	HTpointer here;
	int i, j;
	found = FALSE; 	// Hash Table을 읽기 전, FALSE로 초기화

	// hash code 위치에 어떠한 문자라도 존재하는 경우
	if (HT[hashcode] != NULL) {  
		here = HT[hashcode];
		while (here != NULL && found == FALSE) {		// 현재 가리키는 위치에 문자가 있고 아직 identifier가 발견되지 않은 경우
			found = TRUE;
			i = here->index;
			j = 0;

			while (ST[i] != '\0' && found == TRUE) {	// 문자를 하나씩 비교하면서 identifier의 일치 여부 판단
				// identifier와 일치하지 않는 문자인 경우
				if (ST[i] != identifier[j])		
					found = FALSE;

				// identifier와 일치하는 문자인 경우
				else {					
					i++;
					j++;
				} //End of else

			}
			if (found != TRUE) 
				here = here->next;  // linked list의 다음 identifier로 이동						
		}
		if (here != NULL) {
			if (here->type == "function name") {	// type이 function name인 경우
				here->returntype = returntype;	// 매개변수로 받은 returntype 설정
			}
			else here->returntype = -1;	// function name이 아닌 경우는 -1로 설정
		}
	}
}

// defineIdentType - identifier의 타입(변수, 함수 등)을 저장하는 함수
void defineIdentType(const char *type, const char *identifier)
{
	int length = strlen(identifier);
	int found = FALSE;

	// 해당 identifier의 해시코드 구하기
	int hashcode = 0;		
	for (int i = 0; i < length; i++) {
		hashcode += (int)identifier[i];
	}
	hashcode %= HTsize;

	HTpointer here;
	int i, j;
	found = FALSE; 	// Hash Table을 읽기 전, FALSE로 초기화

	// hash code 위치에 어떠한 문자라도 존재하는 경우
	if (HT[hashcode] != NULL) {  
		here = HT[hashcode];
		while (here != NULL && found == FALSE) {		// 현재 가리키는 위치에 문자가 있고 아직 identifier가 발견되지 않은 경우
			found = TRUE;
			i = here->index;
			j = 0;

			while (ST[i] != '\0' && found == TRUE) {	// 문자를 하나씩 비교하면서 identifier의 일치 여부 판단
				// identifier와 일치하지 않는 문자인 경우
				if (ST[i] != identifier[j])		
					found = FALSE;

				// identifier와 일치하는 문자인 경우
				else {					
					i++;
					j++;
				} //End of else

			}
			if (found != TRUE) 
				here = here->next;  // linked list의 다음 identifier로 이동						
		}
		if (here != NULL) {
			if (here->type == NULL)
				here->type = type;	// identifier의 type을 저장
			else {
				if (found == TRUE)
					printParseError(type, ALREADY_DECLARED);
			}
		}
	}
}
