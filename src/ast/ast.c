#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "src/ast.h"

ASTNode* create_ast_node() {
    ASTNode* node = calloc(1, sizeof(*node));
    node->type = -1;
    return node;
}
void free_ast(ASTNode* ast);

void free_raw_manynodes(many_nodes* nodes) {
    // free(nodes->nodes);
}

void free_forloop(ASTNode* ast) {
    free(ast->fl->iter_ident);
    free_ast(ast->fl->iterable);
    free_ast(ast->fl->body);
}

void free_assignment(ASTNode* ast) {
    free(ast->assign->ident);
    free_ast(ast->assign->expr);
}

void free_manynodes(ASTNode* ast) {
    free_raw_manynodes(ast->nodes);
}

void free_function_call(ASTNode* ast) {
    // if (ast->call->function != NULL)
    //     free_ast(ast->call->function);
    // free_raw_manynodes(ast->call->arguments);
}

void free_function(ASTNode* ast) {
    free(ast->func->name);
    free(ast->func->args);
    free_raw_manynodes(ast->func->body);
}

void free_import(ASTNode* ast) {
    free(ast->string);
}

void free_identifier(ASTNode* ast) {
    free(ast->string);
}

// void free_number(ASTNode( i))

void free_ast(ASTNode* ast) {
    switch (ast->type) {
        case NODE_FORLOOP:       free_forloop(ast);
        case NODE_ASSIGNMENT:    free_assignment(ast);
        case NODE_MANYNODES:     free_manynodes(ast);
        case NODE_FUNCTION_CALL: free_function_call(ast);
        case NODE_FUNCTION:      free_function(ast);
        case NODE_IMPORT:        free_import(ast);
        case NODE_IDENTIFIER:    free_identifier(ast);
        // case NODE_NUMBER:        free_number(ast);
        default:                 assert(0);
    }
    free(ast);
}


char* repr(int type) {
    switch (type) {
        case NODE_FORLOOP:       return "NODE_FORLOOP";
        case NODE_ASSIGNMENT:    return "NODE_ASSIGNMENT";
        case NODE_MANYNODES:     return "NODE_MANYNODES";
        case NODE_FUNCTION_CALL: return "NODE_FUNCTION_CALL";
        case NODE_FUNCTION:      return "NODE_FUNCTION";
        case NODE_IMPORT:        return "NODE_IMPORT";
        case NODE_IDENTIFIER:    return "NODE_IDENTIFIER";
        case NODE_NUMBER:        return "NODE_NUMBER";
        default:                 assert(0);
    }
}