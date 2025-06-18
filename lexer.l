%{
#include <stdio.h>
#include <stdbool.h>
#include "parser.tab.h"
%}

%option noyywrap

AT (?i:at)
DEFINE (?i:define)
EVAL (?i:eval)
FORMULA (?i:formula)
LIST (?i:list)
TABLE (?i:table)
VARLIST (?i:varlist)

AND (?i:and)|&&|&
OR (?i:or)|\|\||\|
NOT (?i:not)|!
XOR (?i:xor)|\^
IMPL =>

NAME [a-zA-Z_][a-zA-Z0-9_]*
BOOL 1|0

%%
{DEFINE} {return DEFINE;}
{FORMULA} {return FORMULA;}
{EVAL} {return EVAL;}
{LIST} {return LIST;}
{TABLE} {return TABLE;}
{VARLIST} {return VARLIST;}

{AT} {return AT;}
{AND} {return AND;}
{OR} {return OR;}
{NOT} {return NOT;}
{XOR} {return XOR;}
{IMPL} {return IMPL;}

{NAME} { yylval.string = strdup(yytext); return NAME;}
{BOOL} {return BOOL;}

[ \t\n] {/* on ignore les espaces en tout genre*/}
. {return yytext[0];}
%%