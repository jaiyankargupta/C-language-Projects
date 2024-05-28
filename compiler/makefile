CC=gcc
CFLAGS=-Wall -Wextra -Werror

compiler: main.o lexer.o parser.o codegen.o
    $(CC) $(CFLAGS) -o compiler main.o lexer.o parser.o codegen.o

main.o: main.c lexer.h parser.h codegen.h
    $(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c lexer.h
    $(CC) $(CFLAGS) -c lexer.c

parser.o: parser.c parser.h lexer.h
    $(CC) $(CFLAGS) -c parser.c

codegen.o: codegen.c codegen.h parser.h
    $(CC) $(CFLAGS) -c codegen.c

clean:
    rm -f *.o compiler
