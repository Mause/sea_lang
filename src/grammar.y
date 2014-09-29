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

void scan_string(const char* str) {
    yy_switch_to_buffer(yy_scan_string(str));
}

ASTNode* res;

%}

%union {
    char* string;
    struct argument_list* arguments;
    struct ASTNode_t* node;
}

%type <arguments> argument_declarations;
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
             ;
%type <string> argument_declaration;// string;

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
            }
          | start_bits start {
              ASTNode* new = create_ast_node();
              new->type = NODE_MANYNODES;
              new->nodes = calloc(1, sizeof(*$$->nodes));
              int num_nodes = res->nodes->num_nodes;
              new->nodes->num_nodes = num_nodes + 1;
              new->nodes->nodes = calloc(new->nodes->num_nodes, sizeof(*new->nodes->nodes));
              int i;
              for (i=0; i<(new->nodes->num_nodes-1); i++) {
                  new->nodes->nodes[i] = res->nodes->nodes[i];
              }
              new->nodes->nodes[new->nodes->num_nodes-1] = $2;
              res = new;
          };

start: function
     //| comment
     | import;

function: FUNC IDENTIFIER OPEN_BRACE argument_declarations CLOSE_BRACE curly_scope {
    $$ = create_function($2, $4, $6->nodes);
};

argument_declarations: {
                           $$ = calloc(1, sizeof(*$$));
                           $$->names = NULL;
                           $$->num_args = 0;
                       } | argument_declarations argument_declaration {
                           $$ = calloc(1, sizeof(*$$));
                           $$->num_args = $1->num_args + 1;
                           $$->names = calloc($$->num_args, sizeof(char*));
                           int i;
                           for (i=0; i<($$->num_args-1); i++) {
                               $$->names[i] = $1->names[i];
                           }
                           $$->names[$$->num_args-1] = $2;
                       };
argument_declaration: IDENTIFIER COMMA {$$ = $1;}
                    | IDENTIFIER       {$$ = $1;};

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


arguments: {$$ = NULL; }
         | arguments argument;
argument : expression COMMA | expression;


expression: IDENTIFIER {
                $$ = create_ast_node();
                $$->type = NODE_IDENTIFIER;
                $$->string = strdup($1);
            }
          | NUMBER {
                $$ = create_ast_node();
                $$->type = NODE_NUMBER;
                $$->string = strdup($1);
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
