// lexer.h
#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_INT,
    TOKEN_ID,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_SEMI,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
} Token;

void init_lexer(const char *source);
Token get_next_token();

#endif
