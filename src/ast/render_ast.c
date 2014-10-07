#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "src/ast/render_ast.h"


void do_indent(int indent) {
    int i;
    for (i=0; i<indent; i++) {
        printf("    ");
    }
}

void render_raw_manynodes(many_nodes* nodes, int indent);
char* render_expression(ASTNode* expr);

void render_forloop(ASTNode* ast, int indent) {
    forloop* fl = ast->fl;

    do_indent(indent); printf("for %s in whatever {\n", fl->iter_ident);

    render_ast(fl->body, indent);

    do_indent(indent); printf("}\n");

}


void render_assignment(ASTNode* ast, int indent) {
    do_indent(indent);
    char* value = render_expression(ast->assign->expr);
    printf("%s = %s;\n", ast->assign->ident, value);
    free(value);
}


void render_declaration(ASTNode* ast, int indent) {
    do_indent(indent);
    printf("var %s;\n", ast->declare->ident);
}


void render_manynodes(ASTNode* ast, int indent) {
    assert(ast->type == NODE_MANYNODES);
    render_raw_manynodes(ast->nodes, indent);
}


void render_raw_manynodes(many_nodes* nodes, int indent) {
    int i;
    do_indent(indent);

    for (i=0; i<nodes->num_nodes; i++) {
        render_ast(nodes->nodes[i], indent+1);
    }
    printf("\n");
}


void render_import(ASTNode* ast, int indent) {
    do_indent(indent);
    printf("import \"%s\";\n", ast->string);
}


char* render_argument_list(argument_list* list) {
    char *buffer = calloc(1024, sizeof(char*));
    int i;
    for(i=0; i < list->num_args; i++) {
        strcat(buffer, list->names[i]);

        if (i != (list->num_args-1)) {
            strcat(buffer, ", ");
        }
    }

    return buffer;
}


void render_function(ASTNode* ast, int indent) {
    char* args = render_argument_list(ast->func->args);

    printf("\n");
    do_indent(indent); printf(
        "func %s(%s) { %d\n",
        ast->func->name,
        args,
        ast->func->body->num_nodes
    );

    free(args);

    render_raw_manynodes(ast->func->body, indent);

    do_indent(indent); printf("}\n");
}

char* render_expression(ASTNode* expr) {
    return (char*)expr;
}

char* render_function_call_to_string(ASTNode* ast) {
    char* function = render_expression(ast->call->function);
    char* function_call = calloc(1024, sizeof(char*));

    char* arguments = "...";
    sprintf(function_call, "%s(%s);", function, arguments);

    free(function);

    return function_call;
}

void render_function_call(ASTNode* ast, int indent) {
    do_indent(indent);
    printf("%s", render_function_call_to_string(ast));
}


void render_ast(ASTNode* ast, int indent) {
    switch(ast->type) {
        RENDER(NODE_FORLOOP, render_forloop);
        RENDER(NODE_ASSIGNMENT, render_assignment);
        RENDER(NODE_MANYNODES, render_manynodes);
        RENDER(NODE_FUNCTION_CALL, render_function_call);
        RENDER(NODE_FUNCTION, render_function);
        RENDER(NODE_IMPORT, render_import);
        RENDER(NODE_DECLARATION, render_declaration);
        default: printf("unknown");
    }
}
