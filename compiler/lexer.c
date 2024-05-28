// lexer.c
#include "lexer.h"
#include <ctype.h>
#include <string.h>

static const char *src;
static size_t src_len;
static size_t idx;

void init_lexer(const char *source) {
    src = source;
    src_len = strlen(source);
    idx = 0;
}

static char peek() {
    return idx < src_len ? src[idx] : '\0';
}

static char advance() {
    return idx < src_len ? src[idx++] : '\0';
}

static void skip_whitespace() {
    while (isspace(peek())) {
        advance();
    }
}

Token get_next_token() {
    Token token;
    skip_whitespace();

    if (isdigit(peek())) {
        token.type = TOKEN_INT;
        int i = 0;
        while (isdigit(peek())) {
            token.lexeme[i++] = advance();
        }
        token.lexeme[i] = '\0';
    } else if (isalpha(peek())) {
        token.type = TOKEN_ID;
        int i = 0;
        while (isalnum(peek())) {
            token.lexeme[i++] = advance();
        }
        token.lexeme[i] = '\0';
    } else {
        char c = advance();
        switch (c) {
            case '=': token.type = TOKEN_ASSIGN; strcpy(token.lexeme, "="); break;
            case '+': token.type = TOKEN_PLUS; strcpy(token.lexeme, "+"); break;
            case '-': token.type = TOKEN_MINUS; strcpy(token.lexeme, "-"); break;
            case '*': token.type = TOKEN_STAR; strcpy(token.lexeme, "*"); break;
            case '/': token.type = TOKEN_SLASH; strcpy(token.lexeme, "/"); break;
            case ';': token.type = TOKEN_SEMI; strcpy(token.lexeme, ";"); break;
            case '\0': token.type = TOKEN_EOF; strcpy(token.lexeme, "EOF"); break;
            default: token.type = TOKEN_EOF; strcpy(token.lexeme, "EOF"); break;
        }
    }
    return token;
}
