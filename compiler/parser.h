// parser.h
#ifndef PARSER_H
#define PARSER_H

typedef struct ASTNode {
    enum { AST_INT, AST_VAR, AST_BINOP, AST_ASSIGN } kind;
    union {
        int int_val;
        char var_name[64];
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char op;
        } binop;
        struct {
            char var_name[64];
            struct ASTNode *expr;
        } assign;
    };
} ASTNode;

ASTNode *parse();

#endif
