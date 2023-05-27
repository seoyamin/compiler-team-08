%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "tn.h"
#include "glob.h"
extern yyerror();
extern printerror();

/*yacc source for Mini C*/
void semantic(int);
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
mini_c 				: translation_unit										{semantic(1);};
translation_unit 	: external_dcl											{semantic(2);}
					| translation_unit external_dcl							{semantic(3);};
external_dcl 		: function_def											{semantic(4);}
					| declaration											{semantic(5);};
function_def 		: function_header compound_st							{semantic(6);};
function_header 	: dcl_spec function_name formal_param					{semantic(7);};
dcl_spec 			: dcl_specifiers										{semantic(8);};
dcl_specifiers 		: dcl_specifier											{semantic(9);}
					| dcl_specifiers dcl_specifier							{semantic(10);};
dcl_specifier 		: type_qualifier										{semantic(11);}
					| type_specifier										{semantic(12);};
type_qualifier 		: TCONST												{semantic(13);};
type_specifier 		: TINT													{semantic(14);}
		 			| TVOID													{semantic(15);};
function_name 		: TIDENT												{printf("TIDENT : %s \n", identStr); defineIdentType("function name", identStr); semantic(16);};
					| TERROR												{printf("!!! lex error... in function name !!!\n");}
formal_param 		: TLEFTPAR TRIGHTPAR 									{semantic(170);}
					| TLEFTPAR error										{yyerrok; printerror("formal_param", NO_RIGHTPAR); semantic(171);};
					| TLEFTPAR formal_param_list TRIGHTPAR					{semantic(172);}
					| TLEFTPAR formal_param_list error						{yyerrok; printerror("formal_param", NO_RIGHTPAR); semantic(173);};
formal_param_list 	: param_dcl												{semantic(20);}
		    		| formal_param_list TCOLON param_dcl 					{semantic(21);}
					| formal_param_list param_dcl error	 					{yyerrok; printerror("formal_param_list", NO_COLON);};
param_dcl 			: dcl_spec declarator									{semantic(22);}
					| dcl_spec error										{yyerrok; printerror("param_dcl", NO_DCL);};
compound_st 		: TLEFTBRACE TRIGHTBRACE								{semantic(231);}
					| TLEFTBRACE error										{yyerrok; printerror("compound_st", NO_RIGHTBRACE); printf("888");}
					| TLEFTBRACE declaration_list TRIGHTBRACE				{semantic(232);}
					| TLEFTBRACE declaration_list error		 				{yyerrok; printerror("compound_st", NO_RIGHTBRACE);printf("777");}
					| TLEFTBRACE statement_list TRIGHTBRACE					{semantic(233);}
					| TLEFTBRACE statement_list error						{yyerrok; printerror("compound_st", NO_RIGHTBRACE); printf("666");}
					| TLEFTBRACE declaration_list statement_list TRIGHTBRACE	{semantic(233);}
					| TLEFTBRACE declaration_list statement_list error		{yyerrok; printerror("compound_st", NO_RIGHTBRACE); printf("555");};
declaration_list 	: declaration											{semantic(26);}
					| declaration_list declaration 							{semantic(27);};
declaration 		: dcl_spec init_dcl_list TSEMI							{semantic(28);}
					| dcl_spec init_dcl_list error 							{yyerrok; printerror("declaration", NO_SEMI);};
init_dcl_list 		: init_declarator										{semantic(29);}
					| init_dcl_list TCOLON init_declarator					{semantic(30);}
					| init_dcl_list init_declarator error					{yyerrok; printerror("init_dcl_list", NO_COLON);};
init_declarator		: declarator											{semantic(31);}
					| declarator TASSIGN TNUMBER							{semantic(32);}
					| declarator TASSIGN error								{yyerrok; printerror("init_declarator", NO_NUM);};
declarator 			: TIDENT												{printf("TIDENT : %s \n", identStr); defineIdentType("integer scalar variable", identStr); semantic(33);}
					| TERROR												{printf("!!! lex error... in declarator !!!\n");}
					| TIDENT TLEFTBRACKET TNUMBER TRIGHTBRACKET				{printf("TIDENT : %s \n", identStr); defineIdentType("integer array variable", identStr); semantic(34);}
					| TERROR TLEFTBRACKET TNUMBER TRIGHTBRACKET				{printf("!!! lex error... in declarator !!!\n");}
					| TIDENT TLEFTBRACKET TRIGHTBRACKET						{printf("TIDENT : %s \n", identStr); defineIdentType("integer array variable", identStr); semantic(34);}
					| TERROR TLEFTBRACKET TRIGHTBRACKET					    {printf("!!! lex error... in declarator !!!\n");}
					| TIDENT TLEFTBRACKET TNUMBER error						{yyerrok; printerror("declarator", NO_RIGHTBRACKET);}
					| TIDENT TLEFTBRACKET TIDENT TRIGHTBRACKET			    {printerror("declarator", NO_NUM);}
					| TIDENT TLEFTBRACKET error								{yyerrok; printerror("declarator", NO_NUM_OR_NO_RIGHTBRACKET);};
statement_list 		: statement												{semantic(39);}
		 			| statement_list statement 								{semantic(40);};
statement 			: compound_st											{semantic(41);}
	   				| expression_st											{semantic(42);}
	   				| if_st													{semantic(43);}
	   				| while_st												{semantic(44);}
	   				| return_st												{semantic(45);};
expression_st 		: TSEMI													{semantic(46);}
					| expression TSEMI										{semantic(46);}
					| expression error										{yyerrok; printerror("expression_st", NO_SEMI);};


if_st 				: TIF TLEFTPAR expression TRIGHTPAR statement	 %prec TLOWERTHANELSE {semantic(49);}
					| TIF TLEFTPAR expression TRIGHTPAR statement TELSE statement 	{semantic(50);}
					| TIF TLEFTPAR expression error statement				{yyerrok; printerror("if_st", NO_RIGHTBRACKET);}
					| TIF TLEFTPAR TRIGHTPAR								{yyerrok; printerror("if_st", NO_EXP);}
					| TIF error	expression TRIGHTPAR statement				{yyerrok; printerror("if_st", NO_LEFTPAR);}; 

while_st 			: TWHILE TLEFTPAR expression TRIGHTPAR statement 		{semantic(51);}
					| TWHILE TLEFTPAR expression error statement			{yyerrok; printerror("while_st", NO_RIGHTPAR);}
					| TWHILE error expression TRIGHTPAR statement			{yyerrok; printerror("while_st", NO_LEFTPAR);};


return_st 			: TRETURN TSEMI											{semantic(52);}								// return;의 경우	
					| TRETURN expression TSEMI								{semantic(52);}								// return 3;의 경우
					| TRETURN expression error								{yyerrok; printerror("return_st", NO_SEMI);}  // return 3의 경우 -> no semi
					| TRETURN error											{yyerrok; printerror("return_st", NO_EXP_OR_NO_SEMI);};  // return 의 경우 -> no return value + no semi

expression 			: assignment_exp										{semantic(53);};

assignment_exp 		: logical_or_exp										{semantic(54);}
					| unary_exp TASSIGN assignment_exp 						{semantic(55);}
					| unary_exp TADDASSIGN assignment_exp					{semantic(56);}
					| unary_exp TSUBASSIGN assignment_exp					{semantic(57);}
					| unary_exp TMULASSIGN assignment_exp					{semantic(58);}
					| unary_exp TDIVASSIGN assignment_exp				 	{semantic(59);}
					| unary_exp TMODASSIGN assignment_exp				 	{semantic(60);}
					| unary_exp TASSIGN error								{yyerrok; printerror("assignment_exp_assign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TADDASSIGN error							{yyerrok; printerror("assignment_exp_addassign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TSUBASSIGN error							{yyerrok; printerror("assignment_exp_subassign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TMULASSIGN error							{yyerrok; printerror("assignment_exp_mulassign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TDIVASSIGN error			 				{yyerrok; printerror("assignment_exp_divassign", NO_RIGHT_SIDE_EXP);}
					| unary_exp TMODASSIGN error							{yyerrok; printerror("assignment_exp_modassign", NO_RIGHT_SIDE_EXP);};

logical_or_exp 		: logical_and_exp										{semantic(61);}
					| logical_or_exp TOR logical_and_exp 					{semantic(62);}
					| logical_or_exp TOR error								{yyerrok; printerror("logical_or_exp", NO_RIGHT_SIDE_EXP);};

logical_and_exp 	: equality_exp											{semantic(63);}
		 			| logical_and_exp TAND equality_exp 					{semantic(64);}
					| logical_and_exp TAND error							{yyerrok; printerror("logical_and_exp", NO_RIGHT_SIDE_EXP);};

equality_exp 		: relational_exp										{semantic(65);}
					| equality_exp TEQUAL relational_exp 					{semantic(66);}
					| equality_exp TNOTEQU relational_exp 					{semantic(67);}
					| equality_exp TEQUAL error								{yyerrok; printerror("equality_exp_equal", NO_RIGHT_SIDE_EXP);}
					| equality_exp TNOTEQU error							{yyerrok; printerror("equality_exp_notequal", NO_RIGHT_SIDE_EXP);};
relational_exp 		: additive_exp 											{semantic(68);}
					| relational_exp TGREAT additive_exp	 				{semantic(69);}
					| relational_exp TLESS additive_exp						{semantic(70);}
					| relational_exp TGREATE additive_exp					{semantic(71);}
					| relational_exp TLESSE additive_exp					{semantic(72);}
					| relational_exp TGREAT	error				 			{yyerrok; printerror("relational_exp_great", NO_RIGHT_SIDE_EXP);}
					| relational_exp TLESS error							{yyerrok; printerror("relational_exp_less", NO_RIGHT_SIDE_EXP);}
					| relational_exp TGREATE error							{yyerrok; printerror("relational_exp_greate", NO_RIGHT_SIDE_EXP);}
					| relational_exp TLESSE error							{yyerrok; printerror("relational_exp_lesse", NO_RIGHT_SIDE_EXP);};
additive_exp 		: multiplicative_exp									{semantic(73);}
					| additive_exp TADD multiplicative_exp					{semantic(74);}
					| additive_exp TSUB multiplicative_exp 					{semantic(75);};
					| additive_exp TADD error								{yyerrok; printerror("additive_exp_add", NO_RIGHT_SIDE_EXP);}
					| additive_exp TSUB error					 			{yyerrok; printerror("additive_exp_sub", NO_RIGHT_SIDE_EXP);};
multiplicative_exp 	: unary_exp												{semantic(76);}
		    		| multiplicative_exp TMUL unary_exp	 					{semantic(77);}
					| multiplicative_exp TDIV unary_exp						{semantic(78);}
				 	| multiplicative_exp TMOD unary_exp						{semantic(79);};
					| multiplicative_exp TMUL error			 				{yyerrok; printerror("multiplicative_exp_mul", NO_RIGHT_SIDE_EXP);}
					| multiplicative_exp TDIV error							{yyerrok; printerror("multiplicative_exp_div", NO_RIGHT_SIDE_EXP);}
				 	| multiplicative_exp TMOD error							{yyerrok; printerror("multiplicative_exp_mod", NO_RIGHT_SIDE_EXP);};
unary_exp 			: postfix_exp											{semantic(80);}
	   				| TSUB unary_exp										{semantic(81);}
	   				| TNOT unary_exp										{semantic(82);}
	   				| TINC unary_exp										{semantic(83);}
	   				| TDEC unary_exp										{semantic(84);};
postfix_exp 		: primary_exp											{semantic(85);}
	      			| postfix_exp TLEFTBRACKET expression TRIGHTBRACKET 	{semantic(86);}
					| postfix_exp TLEFTBRACKET expression error				{yyerrok; printerror("postfix_exp", NO_RIGHTBRACKET);}
					| postfix_exp TLEFTPAR opt_actual_param TRIGHTPAR 		{semantic(87);}
					| postfix_exp TLEFTPAR opt_actual_param error			{yyerrok; printerror("postfix_exp", NO_RIGHTBRACKET);}
	      			| postfix_exp TINC										{semantic(88);}
	      			| postfix_exp TDEC										{semantic(89);};
opt_actual_param 	: actual_param											{semantic(90);}
		  			|														{semantic(91);};
actual_param 		: actual_param_list										{semantic(92);};
actual_param_list 	: assignment_exp										{semantic(93);}
					| actual_param_list TCOLON assignment_exp 				{semantic(94);};
primary_exp 		: TIDENT												{printf("TIDENT : %s \n", identStr); defineIdentType("primary_exp_ident", identStr); semantic(95);}
					| TERROR												{printf("!!! lex error... in primary_exp !!!\n");}
					| TNUMBER												{semantic(96);}
	     			| TLEFTPAR expression TRIGHTPAR							{semantic(97);}
					| TLEFTPAR expression error								{yyerrok; printerror("primary_exp", NO_RIGHTPAR); semantic(97);};
%%
void semantic(int n)
{	
	//printf("reduced rule number = %d\n",n);
}

void defineIdentType(const char *type, const char *identifier)
{
	//printf("\n\n******  Inside the defineIdentType()  ******\n");
	//printf("type: %s, identifier: %s \n\n", type, identifier);
	int length = strlen(identifier);
	int found = FALSE;
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
		here->type = type;
	}
}
