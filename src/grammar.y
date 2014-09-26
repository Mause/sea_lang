%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "src/ast/ast.h"

#define YYDEBUG 1

void yyerror(const char *str) {
    fprintf(stderr,"error: %s\n",str);
}

int yywrap() {
    return 1;
}

void scan_string(const char* str) {
    yy_switch_to_buffer(yy_scan_string(str));
}

ASTNode* res;

%}

%union {
    char* string;
    struct argument_list* arguments;
//    char** string_list;
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
                printf("building first\n");
                res = create_ast_node();
                res->type = NODE_MANYNODES;
                res->nodes = calloc(1, sizeof(*$$->nodes));
                res->nodes->num_nodes = 0;
                res->nodes->nodes = NULL;
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

              printf("should append\n");
          };

start: function
     //| comment
     | import;

function: FUNC IDENTIFIER OPEN_BRACE argument_declarations CLOSE_BRACE curly_scope {
    $$ = create_ast_node();
    $$->type = NODE_FUNCTION;
    $$->func = malloc(sizeof(*$$->func));
    $$->func->name = $2;
    $$->func->args = $4;
    $$->func->body = malloc(sizeof(*$$->func->body));
    $$->func->body->nodes = $6->nodes->nodes;
    $$->func->body->num_nodes = $6->nodes->num_nodes;

    printf("func %s() {}\n", $2);
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

statements:  {
                 $$ = create_ast_node();
                 $$->type = NODE_MANYNODES;
                 $$->nodes = calloc(1, sizeof(*$$->nodes));
                 $$->nodes->num_nodes = 0;
                 $$->nodes->nodes = NULL;
             }
          | statements statement {
              $$ = create_ast_node();
              $$->type = NODE_MANYNODES;
              $$->nodes = calloc(1, sizeof(*$$->nodes));

              $$->nodes->num_nodes = $1->nodes->num_nodes + 1;
              $$->nodes->nodes = calloc($$->nodes->num_nodes, sizeof(*$$->nodes->nodes));

              int i;
              for (i=0; i<($$->nodes->num_nodes-1); i++) {
                $$->nodes->nodes[i] = $1->nodes->nodes[i];
              }
              $$->nodes->nodes[$$->nodes->num_nodes-1] = $2;
          };

statement: assignment SEMICOLON
         | declaration SEMICOLON
         | forloop
         | expression SEMICOLON;

assignment: IDENTIFIER EQUALS expression {
    $$ = create_ast_node();
    $$->type = NODE_ASSIGNMENT;
    $$->assign = malloc(sizeof(*$$->assign));
    $$->assign->ident = strdup($1);
    $$->assign->expr = $3;
};

declaration: VAR assignment {
                $$ = create_ast_node();
                $$->type = NODE_DECLARATION;
                $$->declare = calloc(1, sizeof(*$$->declare));
                $$->declare->ident = $2->assign->ident;
                $$->declare->expr = $2->assign->expr;

//                free_ast($2);
           }
           | VAR IDENTIFIER {
                $$ = create_ast_node();
                $$->type = NODE_DECLARATION;
                $$->declare = calloc(1, sizeof(*$$->declare));
                $$->declare->ident = strdup($2);
                $$->declare->expr = NULL;

//                free_ast($2);
           };

import: IMPORT IDENTIFIER SEMICOLON {
    $$ = create_ast_node();
    $$->type = NODE_IMPORT;
    $$->string = $2;
    printf("Import %s\n", $2);
};


forloop: FOR IDENTIFIER IN expression OPEN_CURLY body CLOSE_CURLY {
    $$ = create_ast_node();
    $$->type = NODE_FORLOOP;
    $$->fl = calloc(1, sizeof(*$$->fl));
    $$->fl->iter_ident = strdup($2);
    $$->fl->iterable = $4;
    $$->fl->body = $6;
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

function_call: IDENTIFIER OPEN_BRACE arguments CLOSE_BRACE {
    $$ = create_ast_node();
    $$->type = NODE_FUNCTION_CALL;
    $$->call = calloc(1, sizeof(*$$->call));
    $$->call->function = NULL;
    $$->call->arguments = NULL;
    printf("%s()\n", $1);
};

//comment: HASH WHATEVER NEWLINE;
%%
