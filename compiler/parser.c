// parser.c
#include <stdio.h>
#include "parser.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>

static Token current_token;

static ASTNode *expr();
static ASTNode *term();
static ASTNode *factor();
static ASTNode *assignment();

static void advance() {
    current_token = get_next_token();
}

static void expect(TokenType type) {
    if (current_token.type != type) {
        fprintf(stderr, "Unexpected token: %s\n", current_token.lexeme);
        exit(1);
    }
    advance();
}

ASTNode *parse() {
    advance();
    return assignment();
}

static ASTNode *assignment() {
    if (current_token.type == TOKEN_ID) {
        ASTNode *node = malloc(sizeof(ASTNode));
        node->kind = AST_ASSIGN;
        strcpy(node->assign.var_name, current_token.lexeme);
        advance();
        expect(TOKEN_ASSIGN);
        node->assign.expr = expr();
        expect(TOKEN_SEMI);
        return node;
    }
    return expr();
}

static ASTNode *expr() {
    ASTNode *node = term();
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        ASTNode *new_node = malloc(sizeof(ASTNode));
        new_node->kind = AST_BINOP;
        new_node->binop.left = node;
        new_node->binop.op = current_token.lexeme[0];
        advance();
        new_node->binop.right = term();
        node = new_node;
    }
    return node;
}

static ASTNode *term() {
    ASTNode *node = factor();
    while (current_token.type == TOKEN_STAR || current_token.type == TOKEN_SLASH) {
        ASTNode *new_node = malloc(sizeof(ASTNode));
        new_node->kind = AST_BINOP;
        new_node->binop.left = node;
        new_node->binop.op = current_token.lexeme[0];
        advance();
        new_node->binop.right = factor();
        node = new_node;
    }
    return node;
}

static ASTNode *factor() {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (current_token.type == TOKEN_INT) {
        node->kind = AST_INT;
        node->int_val = atoi(current_token.lexeme);
        advance();
    } else if (current_token.type == TOKEN_ID) {
        node->kind = AST_VAR;
        strcpy(node->var_name, current_token.lexeme);
        advance();
    } else {
        fprintf(stderr, "Unexpected token: %s\n", current_token.lexeme);
        exit(1);
    }
    return node;
}
