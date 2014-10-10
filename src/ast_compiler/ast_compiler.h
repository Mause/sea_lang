#ifndef AST_COMPILER_H
#define AST_COMPILER_H

#include "./../ast/ast.h"
#include "./../object/code_object.h"

SeaCodeObject* compile_ast(ASTNode* ast);

#endif
