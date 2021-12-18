%{

/* Declarations section */
#define YYSTYPE int
#include "parser.tab.hpp"
#include "output.hpp"
%}

%option yylineno
%option noyywrap
whitespace		([\t\n\r  ])
string  (\"([^\n\r\"\\]|\\[rnt"\\])+\")

%%
void                        return VOID;
int                         return INT;
byte                        return BYTE;
b                           return B;
bool                        return BOOL;
const                       return CONST;
and                         return AND;
or                          return OR;
not                         return NOT;
true                        return TRUE;
false                       return FALSE;
return                      return RETURN;
if                          return IF;
else                        return ELSE;
while                       return WHILE;
break                       return BREAK;
continue                    return CONTINUE;
;                           return SC;
,                           return COMMA;
"("                         return LPAREN;
")"                         return RPAREN;
"{"                         return LBRACE;
"}"                         return RBRACE;
"="                         return ASSIGN;
"<"|">"|"<="|">="           return RELOPLEFT;
"=="|"!="                   return RELOPNONASSOC;
"+"                         return PLUS;
"-"                         return MINUS;
"/"                         return DIV;
"*"                         return MULT;
0|[1-9][0-9]*               return NUM;
[a-zA-Z][0-9a-zA-Z]*        return ID;
{string}                    return STRING;
\/\/[^\r\n]*[\r|\n|\r\n]?   ;
{whitespace}				;
.                       	{output::errorLex(yylineno); exit(0);};
%%