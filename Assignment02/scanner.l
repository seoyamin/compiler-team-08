%{
#include <stdio.h>
#include <stdlib.h>
#include "tn.h" 
#include "symtable.c"
#include "reporterror.c"
%}
letter [a-zA-Z_]
digit [0-9]
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
"="		  return(TASSIGN)
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
{letter}({letter}|{digit})*   { if (yyleng <= 10) {       // 정상 : symtable.c
									symbol(yytext);		
									return(TIDENT); 
								}
								else {
									return(yytext[0]);    // err(overlen) : reporterror.c
								}
							  }     
{digit}+   return(TNUMBER);
"//".*            ;
[ \t\n]            ;
.            return(yytext[0]);    // 에러인 경우 : reporterror.c
%%
int yywrap()
{   printf("[1983024 Choi Mingyo, 2017007 Kim Minseo, 2173109 Jeong Eunbi]\n");
   return 1;
}                                                          