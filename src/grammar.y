%{
#include <stdio.h>
#define YYDEBUG 1
extern int yydebug;
//yydebug = 1;

void yyerror(const char *str) {
    fprintf(stderr,"error: %s\n",str);
}

int yywrap() {
    return 1;
}

void scan_string(const char* str) {
    yy_switch_to_buffer(yy_scan_string(str));
}

%}

%union {
    char* string;
}

%token <string>
    IDENTIFIER NUMBER WHATEVER

    CLOSE_CURLY OPEN_CURLY
    CLOSE_BRACE OPEN_BRACE
    FUNC VAR IMPORT IN FOR
    COMMA EQUALS HASH NEWLINE SEMICOLON
    ;

%start start_bits;

%%
start_bits:
          | start_bits start;

start: function
//     | comment
     | import;

function: FUNC IDENTIFIER OPEN_BRACE argument_declarations CLOSE_BRACE OPEN_CURLY body CLOSE_CURLY {
    printf("func %s() {}\n", $2);
};

argument_declarations: | arguments argument_declaration;
argument_declaration: IDENTIFIER COMMA
                    | IDENTIFIER;

body: statements;

statements:
          | statements statement;

statement: assignment SEMICOLON
         | declaration SEMICOLON
         | forloop
         | expression SEMICOLON;

assignment: IDENTIFIER EQUALS expression {
    printf("%s equals %s\n", $1, $2);
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
    printf("Import %s\n", $2);
};

forloop: FOR IDENTIFIER IN expression OPEN_CURLY body CLOSE_CURLY {
    printf("for %s in something\n", $2); //, $4);
};

arguments:
         | arguments expression;

expression: IDENTIFIER
          | NUMBER;
          | expression '+' expression
          | expression '-' expression
          | expression '*' expression
          | function_call;

function_call: IDENTIFIER OPEN_BRACE arguments CLOSE_BRACE {
    printf("%s()\n", $1);
};

//comment: HASH WHATEVER NEWLINE;
%%
