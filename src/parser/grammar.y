%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "src/ast/ast.h"
#include "src/grammar_handlers.h"

#define YYDEBUG 1

void yyerror(const char *str) {
    fprintf(stderr, "error: %s\n", str);
}

ASTNode* res;

%}

%union {
    char* string;
    struct ASTNode_t* node;
}

%type <node> forloop
             arguments
             assignment
             declaration
             import
             body
             expression
             function
             function_call
             start
             start_bits
             statement
             statements
             curly_scope
             argument
             argument_declaration
             argument_declarations
             ;

%token <string>
    IDENTIFIER NUMBER WHATEVER

    CLOSE_CURLY OPEN_CURLY
    CLOSE_BRACE OPEN_BRACE
    FUNC VAR IMPORT IN FOR QUOTE
    COMMA EQUALS HASH NEWLINE SEMICOLON
    ;

%start start_bits;

%%
start_bits: {
                res = create_empty_manynodes();
                $$ = NULL;
            }
          | start_bits start {
                res = append_to_manynodes(res, $2);
          };

start: function
     //| comment
     | import;

function: FUNC IDENTIFIER OPEN_BRACE argument_declarations CLOSE_BRACE curly_scope {
    $$ = create_function($2, $4->nodes, $6->nodes);
    free($4);
    free($6);
};

argument_declarations: {
                          $$ = create_empty_manynodes();
                       } | argument_declarations argument_declaration {
                          $$ = append_to_manynodes($1, $2);
                       };
argument_declaration: IDENTIFIER COMMA { $$ = create_identifier($1); }
                    | IDENTIFIER       { $$ = create_identifier($1); };

curly_scope: OPEN_CURLY body CLOSE_CURLY {$$ = $2;};
body: statements;

statements:                      { $$ = create_empty_manynodes(); }
          | statements statement { $$ = append_to_manynodes($1, $2); };

statement: assignment SEMICOLON
         | declaration SEMICOLON
         | forloop
         | expression SEMICOLON;

assignment: IDENTIFIER EQUALS expression {
    $$ = create_assignment($1, $3);
};

declaration: VAR assignment { $$ = create_declaration_from_assignment($2); }
           | VAR IDENTIFIER { $$ = create_declaration($2); };

import: IMPORT IDENTIFIER SEMICOLON { $$ = create_import($2); };


forloop: FOR IDENTIFIER IN expression OPEN_CURLY body CLOSE_CURLY {
    $$ = create_forloop($2, $4, $6);
};


arguments:                    { $$ = create_empty_manynodes(); }
         | arguments argument { $$ = append_to_manynodes($1, $2); };
argument : expression COMMA | expression;


expression: IDENTIFIER {
              $$ = create_identifier($1);
            }
          | NUMBER {
            $$ = create_number($1);
          }
//          | expression '+' expression
//          | expression '-' expression
//          | expression '*' expression
          | function_call
        //  | string
          ;

//string: QUOTE WHATEVER QUOTE { $$ = $2; };

//function_call: IDENTIFIER OPEN_BRACE arguments CLOSE_BRACE {
function_call: expression OPEN_BRACE arguments CLOSE_BRACE {
    $$ = create_function_call($1, $3);
};

//comment: HASH WHATEVER NEWLINE;
%%
