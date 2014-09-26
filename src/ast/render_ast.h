#ifndef RENDER_AST_H
#define RENDER_AST_H

#include "src/ast/ast.h"

#define RENDER(enum_val, function) {case enum_val: function(ast, indent); break;}

void render_ast(ASTNode* ast, int indent);

#endif
