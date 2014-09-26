#ifndef GRAMMAR_HANDLERS_H
#define GRAMMAR_HANDLERS_H

#include "src/ast/ast.h"

ASTNode* create_function(char* name, struct argument_list* args, many_nodes* body);
ASTNode* create_assignment(char* ident, ASTNode* expr);
ASTNode* create_function_call(ASTNode* function, ASTNode* arguments);
ASTNode* create_import(char* ident);
ASTNode* create_declaration_from_assignment(ASTNode* assignment);
ASTNode* create_declaration(char* ident);
ASTNode* append_to_manynodes(ASTNode* manynodes, ASTNode* new_node);
ASTNode* create_empty_manynodes();
ASTNode* create_forloop(char* ident, ASTNode* iterable, ASTNode* body);

#endif
