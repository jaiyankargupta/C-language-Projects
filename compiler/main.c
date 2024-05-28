// main.c
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *source = "x = 10 + 20 * 30;";

    init_lexer(source);
    ASTNode *ast = parse();
    generate_code(ast);

    return 0;
}
