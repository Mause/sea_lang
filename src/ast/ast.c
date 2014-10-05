#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "src/ast/ast.h"

ASTNode* create_ast_node(void) {
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

void free_declaration(ASTNode* ast) {
    free(ast->declare->ident);
    if (ast->declare->expr != NULL) {
        free(ast->declare->expr);
    }
}

// void free_number(ASTNode( i))

void free_ast(ASTNode* ast) {
    switch (ast->type) {
        case NODE_FORLOOP:       free_forloop(ast);       break;
        case NODE_ASSIGNMENT:    free_assignment(ast);    break;
        case NODE_MANYNODES:     free_manynodes(ast);     break;
        case NODE_FUNCTION_CALL: free_function_call(ast); break;
        case NODE_FUNCTION:      free_function(ast);      break;
        case NODE_IMPORT:        free_import(ast);        break;
        case NODE_IDENTIFIER:    free_identifier(ast);    break;
        case NODE_DECLARATION:   free_declaration(ast);   break;
        // case NODE_NUMBER:        free_number(ast);        break;
        default:                 assert("ASTNode type not declared" == 0);
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
        case NODE_DECLARATION:   return "NODE_DECLARATION";
        default:                 assert(0);
    }
}
