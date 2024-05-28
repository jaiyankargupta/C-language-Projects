#include "codegen.h"
#include <stdio.h>

static void gen_expr(ASTNode *node);

void generate_code(ASTNode *node) {
    if (node->kind == AST_ASSIGN) {
        gen_expr(node->assign.expr);
        printf("MOV [%s], R0\n", node->assign.var_name);
    } else {
        gen_expr(node);
    }
}

static void gen_expr(ASTNode *node) {
    if (node->kind == AST_INT) {
        printf("MOV R0, %d\n", node->int_val);
    } else if (node->kind == AST_VAR) {
        printf("MOV R0, [%s]\n", node->var_name);
    } else if (node->kind == AST_BINOP) {
        gen_expr(node->binop.left);
        printf("PUSH R0\n");
        gen_expr(node->binop.right);
        printf("POP R1\n");
        switch (node->binop.op) {
            case '+': printf("ADD R0, R1, R0\n"); break;
            case '-': printf("SUB R0, R1, R0\n"); break;
            case '*': printf("MUL R0, R1, R0\n"); break;
            case '/': printf("DIV R0, R1, R0\n"); break;
        }
    }
}
