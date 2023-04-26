%{
#include <stdio.h>
#include <stdlib.h>
#include "tn.h" 
#include "glob.h"
extern SymbolTable();
extern reporterror();
%}
letter [a-zA-Z_]
digit [0-9]
op "+" | "+="
operator "+"|"-"|"*"|"/"|"%"|"+="|"-="|"*="|"/="|"%="|"!"|"&&"|"||"|"=="|"!="|"<"|">"|"<="|">="|"++"|"--" 
ssymbol "(" | ")" | "," | "{" | "}" | "[" | "]" | "|" | ";"
%%
"const"   return(TCONST); 
"else"    return(TELSE);
"if"      return(TIF);
"int"     return(TINT);
"return"  return(TRETURN);
"void"    return(TVOID);
"while"   return(TWHILE);
"+"		  return(TADD);
"-"		  return(TSUB);
"*"		  return(TMUL);
"/"		  return(TDIV);
"%"		  return(TMOD);
"="		  return(TASSIGN);
"+="      return(TADDASSIGN);
"-="      return(TSUBASSIGN);
"*="      return(TMULASSIGN);
"/="      return(TDIVASSIGN);
"%="      return(TMODASSIGN);
"!"		  return(TNOT);
"&&"      return(TAND);
"||"      return(TOR);
"=="      return(TEQUAL);
"!="      return(TNOTEQU);
"<"		  return(TLESS);
">"		  return(TGREAT);
"<="      return(TLESSE);
">="      return(TGREATE);
"++"      return(TINC);
"--"      return(TDEC);
"("		  return(TLEFTPAR);
")"		  return(TRIGHTPAR);
","		  return(TCOLON);
"{"		  return(TLEFTBRACE);
"}"		  return(TRIGHTBRACE);
"["		  return(TLEFTBRACKET);
"]"		  return(TRIGHTBRACKET);
";"		  return(TSEMI);
"\n"	  linenum = linenum + 1;
{letter}({letter}|{digit})*   { if (yyleng <= 10) {			// 정상 identifier
									SymbolTable(yytext);		
									return(TIDENT); 
								}
								else {						// 에러 발생 (OVERLEN)
									reporterror(yytext);
									return(TERROR);    
								}
							  } 
{digit}+   return(TNUMBER);
{digit}+{letter}*	{
						reporterror(yytext);	// 에러 발생 (ILLIDENT)
						return(TERROR);  
					}
"//".*     ;
[ \t]      ;
.            {
				reporterror(yytext);		// 에러 발생 (ILLCHAR)
				return(TERROR);    
			 }
%%
int yywrap()
{   
    return 1;
}                                                          