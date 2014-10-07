#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "src/grammar_handlers.h"


ASTNode* create_function(char* name, struct argument_list* args, many_nodes* body) {
    ASTNode* function = create_ast_node();
    function->type = NODE_FUNCTION;
    function->func = calloc(1, sizeof(*function->func));
    function->func->name = name;
    function->func->args = args;
    function->func->body = body;

    printf("func %s() {}\n", name);

    return function;
}


ASTNode* create_assignment(char* ident, ASTNode* expr) {
   ASTNode* assignment = create_ast_node();
   assignment->type = NODE_ASSIGNMENT;

   assignment->assign = calloc(1, sizeof(*assignment->assign));
   assignment->assign->ident = strdup(ident);
   assignment->assign->expr = expr;

   return assignment;
}

ASTNode* create_function_call(ASTNode* function, ASTNode* arguments) {
    ASTNode* function_call = create_ast_node();
    function_call->type = NODE_FUNCTION_CALL;
    function_call->call = calloc(1, sizeof(*function_call->call));
    function_call->call->function = function;
    if (arguments != NULL) {
        function_call->call->arguments = arguments->nodes;
    } else {
        function_call->call->arguments = NULL;
    }
    printf("%s()\n", function);

    return function_call;
}

ASTNode* create_import(char* ident) {
    ASTNode* import = create_ast_node();
    import->type = NODE_IMPORT;
    import->string = ident;
    printf("Import %s\n", ident);

    return import;
}


ASTNode* create_declaration_from_assignment(ASTNode* assignment) {
    ASTNode* declaration = create_ast_node();
    declaration->type = NODE_DECLARATION;
    declaration->declare = calloc(1, sizeof(*declaration->declare));
    declaration->declare->ident = assignment->assign->ident;
    declaration->declare->expr = assignment->assign->expr;

    return declaration;
}


ASTNode* create_declaration(char* ident) {
    ASTNode* declaration = create_ast_node();
    declaration->type = NODE_DECLARATION;
    declaration->declare = calloc(1, sizeof(*declaration->declare));
    declaration->declare->ident = strdup(ident);
    declaration->declare->expr = NULL;

    return declaration;
}


ASTNode* create_empty_manynodes(void) {
    ASTNode* manynodes = create_ast_node();
    manynodes->type = NODE_MANYNODES;
    manynodes->nodes = calloc(1, sizeof(*manynodes->nodes));
    manynodes->nodes->num_nodes = 0;
    manynodes->nodes->nodes = NULL;

    return manynodes;
}

ASTNode* append_to_manynodes(ASTNode* manynodes, ASTNode* new_node) {
    ASTNode* new_many = create_ast_node();
    new_many->type = NODE_MANYNODES;
    new_many->nodes = calloc(1, sizeof(*new_many->nodes));

    new_many->nodes->num_nodes = manynodes->nodes->num_nodes + 1;
    new_many->nodes->nodes = calloc(new_many->nodes->num_nodes, sizeof(*new_many->nodes->nodes));

    int i;
    for (i=0; i<(new_many->nodes->num_nodes-1); i++) {
        new_many->nodes->nodes[i] = manynodes->nodes->nodes[i];
    }
    new_many->nodes->nodes[new_many->nodes->num_nodes-1] = new_node;

    free(manynodes->nodes->nodes);
    free(manynodes->nodes);
    free(manynodes);

    return new_many;
}

ASTNode* create_forloop(char* ident, ASTNode* iterable, ASTNode* body) {
    ASTNode* forloop = create_ast_node();
    forloop->type = NODE_FORLOOP;
    forloop->fl = calloc(1, sizeof(*forloop->fl));
    forloop->fl->iter_ident = strdup(ident);
    forloop->fl->iterable = iterable;
    forloop->fl->body = body;

    return forloop;
}
