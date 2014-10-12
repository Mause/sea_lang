%{
#include <stdio.h>
#include "build/parser/grammar.h"
%}

%option noyywrap

%x COMMENT

%%
[0-9]+                  return NUMBER;
\{                      return OPEN_CURLY;
\}                      return CLOSE_CURLY;
;                       return SEMICOLON;
\n                      { yylloc.last_line++; };
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
<INITIAL>#              BEGIN(COMMENT);
<COMMENT>"\n"           BEGIN(INITIAL);
<COMMENT>.              {};
\"                      return QUOTE;

[a-zA-Z_][_a-zA-Z0-9]*  {
                            yylval.string=strdup(yytext);
                            return IDENTIFIER;
                        }

%%
