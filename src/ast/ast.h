#ifndef AST_H
#define AST_H

struct forloop_t;

enum NodeType {
    NODE_FORLOOP,
    NODE_ASSIGNMENT,
    NODE_MANYNODES,
    NODE_FUNCTION_CALL,
    NODE_FUNCTION,
    NODE_IMPORT,
    NODE_IDENTIFIER,
    NODE_NUMBER,
    NODE_STRING_LITERAL
};

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
    int num_nodes;
    struct ASTNode_t** nodes;
} many_nodes;

typedef struct argument_list {
    int num_args;
    char** names;
} argument_list;

typedef struct {
    struct ASTNode_t* function;
    many_nodes* arguments;
} function_call;

typedef struct {
    char* name;
    argument_list* args;
    many_nodes* body;
} function;

typedef struct ASTNode_t {
    int type;
    union {
        forloop* fl;
        char* string;
        assignment* assign;
        many_nodes* nodes;
        function_call* call;
        function* func;
    };
} ASTNode;

ASTNode* create_ast_node();
char* repr(int type);

#endif
