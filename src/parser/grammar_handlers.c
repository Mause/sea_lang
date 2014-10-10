#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "./grammar_handlers.h"


ASTNode* create_function(char* name, many_nodes* args, many_nodes* body) {
    ASTNode* function = create_ast_node(NODE_FUNCTION);
    function->func = calloc(1, sizeof(*function->func));
    function->func->name = name;
    function->func->args = args;
    function->func->body = body;

    return function;
}


ASTNode* create_assignment(char* ident, ASTNode* expr) {
   ASTNode* assignment = create_ast_node(NODE_ASSIGNMENT);

   assignment->assign = calloc(1, sizeof(*assignment->assign));
   assignment->assign->ident = ident;
   assignment->assign->expr = expr;

   return assignment;
}

ASTNode* create_function_call(ASTNode* function, ASTNode* arguments) {
    ASTNode* function_call = create_ast_node(NODE_FUNCTION_CALL);
    function_call->call = calloc(1, sizeof(*function_call->call));

    function_call->call->function = function;

    function_call->call->arguments = arguments->nodes;
    free(arguments);

    return function_call;
}

ASTNode* create_import(char* ident) {
    ASTNode* import = create_ast_node(NODE_IMPORT);
    import->string = ident;

    return import;
}


ASTNode* create_declaration_from_assignment(ASTNode* assignment) {
    ASTNode* declaration = create_ast_node(NODE_DECLARATION);
    declaration->declare = calloc(1, sizeof(*declaration->declare));
    declaration->declare->ident = assignment->assign->ident;
    declaration->declare->expr = assignment->assign->expr;

    free(assignment->assign);
    free(assignment);

    return declaration;
}


ASTNode* create_number(char* string) {
    ASTNode* number = create_ast_node(NODE_NUMBER);
    number->string = string;

    return number;
}


ASTNode* create_identifier(char* string) {
    ASTNode* ident = create_ast_node(NODE_IDENTIFIER);
    ident->string = string;

    return ident;
}


ASTNode* create_declaration(char* ident) {
    ASTNode* declaration = create_ast_node(NODE_DECLARATION);
    declaration->declare = calloc(1, sizeof(*declaration->declare));
    declaration->declare->ident = ident;
    declaration->declare->expr = NULL;

    return declaration;
}


ASTNode* create_empty_manynodes(void) {
    ASTNode* manynodes = create_ast_node(NODE_MANYNODES);
    manynodes->nodes = calloc(1, sizeof(*manynodes->nodes));

    return manynodes;
}

ASTNode* append_to_manynodes(ASTNode* manynodes, ASTNode* new_node) {
    manynodes->nodes->num_nodes++;
    manynodes->nodes->nodes = realloc(
        manynodes->nodes->nodes,
        manynodes->nodes->num_nodes * sizeof(*manynodes->nodes->nodes)
    );

    manynodes->nodes->nodes[manynodes->nodes->num_nodes-1] = new_node;

    return manynodes;
}

ASTNode* create_forloop(char* ident, ASTNode* iterable, ASTNode* body) {
    ASTNode* forloop = create_ast_node(NODE_FORLOOP);
    forloop->fl = calloc(1, sizeof(*forloop->fl));
    forloop->fl->iter_ident = ident;
    forloop->fl->iterable = iterable;
    forloop->fl->body = body;

    return forloop;
}
