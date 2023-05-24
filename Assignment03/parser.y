%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "tn.h"
#include "glob.h"
extern yyerror();

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
					| translation_unit external_dcl						{semantic(3);};
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
formal_param 		: TLEFTPAR opt_formal_param TRIGHTPAR 					{semantic(17);}
					| TLEFTPAR opt_formal_param error						{yyerrok; yyerror("formal_param_no_right parenthesis\n"); semantic(17);};
opt_formal_param 	: formal_param_list										{semantic(18);}
					|														{semantic(19);};
formal_param_list 	: param_dcl												{semantic(20);}
		    		| formal_param_list TCOLON param_dcl 					{semantic(21);};
param_dcl 			: dcl_spec declarator									{semantic(22);};
compound_st 		: TLEFTBRACE opt_dcl_list opt_stat_list TRIGHTBRACE 	{semantic(23);};
opt_dcl_list 		: declaration_list										{semantic(24);}
					|														{semantic(25);};
declaration_list 	: declaration											{semantic(26);}
					| declaration_list declaration 						{semantic(27);};
declaration 		: dcl_spec init_dcl_list TSEMI							{semantic(28);}
					| dcl_spec init_dcl_list error 						{yyerrok; yyerror("declaration_no_semi\n");};
init_dcl_list 		: init_declarator										{semantic(29);}
					| init_dcl_list TCOLON init_declarator					{semantic(30);};
init_declarator		: declarator											{semantic(31);}
					| declarator TASSIGN TNUMBER							{semantic(32);};
declarator 			: TIDENT												{printf("TIDENT : %s \n", identStr); defineIdentType("integer scalar variable", identStr); semantic(33);}
					| TIDENT TLEFTBRACKET TRIGHTBRACKET					{printf("TIDENT : %s \n", identStr); defineIdentType("integer array variable", identStr); semantic(34);}
					| TIDENT TLEFTBRACKET error								{yyerrok; yyerror("declarator_no_number\n");}
	     			| TIDENT TLEFTBRACKET TNUMBER TRIGHTBRACKET			{printf("TIDENT : %s \n", identStr); defineIdentType("integer array variable", identStr); semantic(34);}
					| TIDENT TLEFTBRACKET TNUMBER error					{yyerrok; yyerror("declarator_no_right bracket\n");};
opt_stat_list 		: statement_list										{semantic(37);}
		 			|														{semantic(38);};
statement_list 		: statement												{semantic(39);}
		 			| statement_list statement 								{semantic(40);};
statement 			: compound_st											{semantic(41);}
	   				| expression_st											{semantic(42);}
	   				| if_st													{semantic(43);}
	   				| while_st												{semantic(44);}
	   				| return_st												{semantic(45);};
expression_st 		: TSEMI													{semantic(46);}
					| error													{yyerrok; yyerror("expression_st_no_semi\n");}
					| expression TSEMI										{semantic(46);}
					| expression error										{yyerrok; yyerror("expression_st_no_semi\n");};
if_st 				: TIF TLEFTPAR expression TRIGHTPAR statement	 %prec TLOWERTHANELSE {semantic(49);}
					| TIF TLEFTPAR expression TRIGHTPAR statement TELSE statement 	{semantic(50);}
					| TIF TLEFTPAR expression error statement				{yyerrok; yyerror("if_st_no_right bracket\n");};
while_st 			: TWHILE TLEFTPAR expression TRIGHTPAR statement 		{semantic(51);}
					| TWHILE TLEFTPAR expression error statement			{yyerrok; yyerror("while_st_no_right parenthesis\n");};
return_st 			: TRETURN TSEMI							{semantic(52);}
					| TRETURN error							{yyerrok; yyerror("return_st_no_semi\n");};
					| TRETURN expression TSEMI							{semantic(52);}
					| TRETURN expression error							{yyerrok; yyerror("return_st_no_semi\n");};
expression 			: assignment_exp										{semantic(53);};
assignment_exp 		: logical_or_exp										{semantic(54);}
					| unary_exp TASSIGN assignment_exp 						{semantic(55);}
					| unary_exp TADDASSIGN assignment_exp					{semantic(56);}
					| unary_exp TSUBASSIGN assignment_exp					{semantic(57);}
					| unary_exp TMULASSIGN assignment_exp					{semantic(58);}
					| unary_exp TDIVASSIGN assignment_exp				 	{semantic(59);}
					| unary_exp TMODASSIGN assignment_exp				 	{semantic(60);};
logical_or_exp 		: logical_and_exp										{semantic(61);}
					| logical_or_exp TOR logical_and_exp 					{semantic(62);};
logical_and_exp 	: equality_exp											{semantic(63);}
		 			| logical_and_exp TAND equality_exp 					{semantic(64);};
equality_exp 		: relational_exp										{semantic(65);}
					| equality_exp TEQUAL relational_exp 					{semantic(66);}
					| equality_exp TNOTEQU relational_exp 					{semantic(67);};
relational_exp 		: additive_exp 											{semantic(68);}
					| relational_exp TGREAT additive_exp	 				{semantic(69);}
					| relational_exp TLESS additive_exp						{semantic(70);}
					| relational_exp TGREATE additive_exp					{semantic(71);}
					| relational_exp TLESSE additive_exp					{semantic(72);};
additive_exp 		: multiplicative_exp									{semantic(73);}
					| additive_exp TADD multiplicative_exp					{semantic(74);}
					| additive_exp TSUB multiplicative_exp 					{semantic(75);};
multiplicative_exp 	: unary_exp												{semantic(76);}
		    		| multiplicative_exp TMUL unary_exp	 					{semantic(77);}
					| multiplicative_exp TDIV unary_exp						{semantic(78);}
				 	| multiplicative_exp TMOD unary_exp						{semantic(79);};
unary_exp 			: postfix_exp											{semantic(80);}
	   				| TSUB unary_exp										{semantic(81);}
	   				| TNOT unary_exp										{semantic(82);}
	   				| TINC unary_exp										{semantic(83);}
	   				| TDEC unary_exp										{semantic(84);};
postfix_exp 		: primary_exp											{semantic(85);}
	      			| postfix_exp TLEFTBRACKET expression TRIGHTBRACKET 	{semantic(86);}
					| postfix_exp TLEFTBRACKET expression error				{yyerrok; yyerror("postfix_exp_no_right bracket\n");}
					| postfix_exp TLEFTPAR opt_actual_param TRIGHTPAR 		{semantic(87);}
					| postfix_exp TLEFTPAR opt_actual_param error			{yyerrok; yyerror("postfix_exp_opt_actual_param_no_right bracket\n");}
	      			| postfix_exp TINC										{semantic(88);}
	      			| postfix_exp TDEC										{semantic(89);};
opt_actual_param 	: actual_param											{semantic(90);}
		  			|														{semantic(91);};
actual_param 		: actual_param_list										{semantic(92);};
actual_param_list 	: assignment_exp										{semantic(93);}
					| actual_param_list TCOLON assignment_exp 				{semantic(94);};
primary_exp 		: TIDENT												{printf("TIDENT : %s \n", identStr); defineIdentType("primary expression", identStr); semantic(95);}
	     			| TNUMBER												{semantic(96);}
	     			| TLEFTPAR expression TRIGHTPAR							{semantic(97);}
					| TLEFTPAR expression error								{yyerrok; yyerror("primary_exp_no_rightpar\n"); semantic(97);};
%%
void semantic(int n)
{	
	printf("reduced rule number = %d\n",n);
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
