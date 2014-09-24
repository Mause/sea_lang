%{
#include <stdio.h>
#include "build/grammar.h"
%}

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
\"                      printf("QUOTE ");

[a-zA-Z][a-zA-Z0-9]*    {
                            yylval.string=strdup(yytext);
                            return IDENTIFIER;
                        }

%%
