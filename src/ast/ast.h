#ifndef AST_H
#define AST_H

#include "build/parser/grammar.h"

struct forloop_t;

typedef enum {
    NODE_FORLOOP,
    NODE_ASSIGNMENT,
    NODE_MANYNODES,
    NODE_FUNCTION_CALL,
    NODE_FUNCTION,
    NODE_IMPORT,
    NODE_IDENTIFIER,
    NODE_NUMBER,
    NODE_STRING_LITERAL,
    NODE_DECLARATION,
    NODE_ERROR
} NodeType;

struct ASTNode_t;

typedef struct {
    char* iter_ident;
    struct ASTNode_t* iterable;
    struct ASTNode_t* body;
} forloop;

typedef struct {
    char* ident;
    struct ASTNode_t* expr;
} assignment;

typedef struct {
    char* ident;
    struct ASTNode_t* expr;
} declaration;

typedef struct {
    int num_nodes;
    struct ASTNode_t** nodes;
} many_nodes;

typedef struct {
    struct ASTNode_t* function;
    many_nodes* arguments;
} function_call;

typedef struct {
    char* name;
    many_nodes* args;
    many_nodes* body;
} function;

typedef struct {
    char* error_location;
    YYLTYPE lloc;
    char* message;
} error;

typedef struct ASTNode_t {
    NodeType type;
    union {
        forloop* fl;
        char* string;
        assignment* assign;
        many_nodes* nodes;
        function_call* call;
        function* func;
        declaration* declare;
        error* err;
    };
} ASTNode;

ASTNode* create_ast_node(NodeType type);
char* repr(int type);
many_nodes* grab_errors(ASTNode* ast);

#endif
