%{
#include <stdio.h>
#include "build/parser/grammar.h"
%}

%option noyywrap

%%
[0-9]+                  return NUMBER;
\{                      return OPEN_CURLY;
\}                      return CLOSE_CURLY;
;                       return SEMICOLON;
\n                      /*;return NEWLINE;*/
[ \t]+                  /* ignore whitespace */;

\)                      return CLOSE_BRACE;
\(                      return OPEN_BRACE;
var                     return VAR;
func                    return FUNC;
import                  return IMPORT;
in                      return IN;
for                     return FOR;

,                       return COMMA;
=                       return EQUALS;
#                       return HASH;
\"                      return QUOTE;

[a-zA-Z_][_a-zA-Z0-9]*  {
                            yylval.string=strdup(yytext);
                            return IDENTIFIER;
                        }

%%
