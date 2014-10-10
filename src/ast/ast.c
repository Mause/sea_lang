#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "src/ast/ast.h"
#include "src/parser/grammar_handlers.h"

ASTNode* create_ast_node(NodeType type) {
    ASTNode* node = calloc(1, sizeof(*node));
    node->type = type;
    return node;
}
void free_ast(ASTNode* ast);

void free_raw_manynodes(many_nodes* nodes) {
    int i;
    for (i=0; i<nodes->num_nodes; i++) {
        free_ast(nodes->nodes[i]);
    }
    free(nodes->nodes);
    free(nodes);
}

void free_forloop(ASTNode* ast) {
    free(ast->fl->iter_ident);
    free_ast(ast->fl->iterable);
    free_ast(ast->fl->body);
    free(ast->fl);
}

void free_assignment(ASTNode* ast) {
    free(ast->assign->ident);
    free_ast(ast->assign->expr);
    free(ast->assign);
}

void free_manynodes(ASTNode* ast) {
    free_raw_manynodes(ast->nodes);
}

void free_function_call(ASTNode* ast) {
    if (ast->call->function != NULL)
        free_ast(ast->call->function);

    if (ast->call->arguments != NULL) {
        free_raw_manynodes(ast->call->arguments);
    }
    free(ast->call);
}

void free_function(ASTNode* ast) {
    free(ast->func->name);
    free_raw_manynodes(ast->func->args);
    free_raw_manynodes(ast->func->body);
    free(ast->func);
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
    free(ast->declare);
}

void free_string_literal(ASTNode* ast) {
    free(ast->string);
}

void free_number(ASTNode* ast) {
    free(ast->string);
}

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
        case NODE_NUMBER:        free_number(ast);        break;
        case NODE_STRING_LITERAL: free_string_literal(ast); break;
        case NODE_ERROR:         break;
        default: {
            printf("Unknown type; %d\n", ast->type);
            assert("ASTNode type not declared" == 0);
        }
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

many_nodes* grab_errors(ASTNode* ast) {
    ASTNode* errors = create_empty_manynodes();

    int i;

    for (i=0; i<ast->nodes->num_nodes; i++) {
        ASTNode* cur_node = ast->nodes->nodes[i];

        switch(cur_node->type) {
        case NODE_MANYNODES: {
            many_nodes* to_add = grab_errors(cur_node);

            errors->nodes->num_nodes += to_add->num_nodes;
            errors->nodes->nodes = realloc(
                errors->nodes->nodes,
                sizeof(*errors->nodes->nodes)
            );

            ASTNode* add_at_pos = errors->nodes->nodes[errors->nodes->num_nodes - to_add->num_nodes];
            assert(add_at_pos == NULL);
            memcpy(
                add_at_pos,
                to_add->nodes,
                to_add->num_nodes * sizeof(*to_add->nodes)
            );

        }
        case NODE_ERROR: {
            errors = append_to_manynodes(errors, cur_node);
        }
        }
    }

    many_nodes* errors_actual = errors->nodes;
    free(errors);
    return errors_actual;
}
