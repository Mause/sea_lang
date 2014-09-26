#ifndef COMPILE_H
#define COMPILE_H

#include "src/stack.h"
#include "src/ast/ast.h"

callstack* compile(ASTNode* ast);

#endif
