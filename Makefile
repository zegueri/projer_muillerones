CC=cc

all: parser.y lexer.l
	bison -d -g -v -t parser.y
	flex -dTv lexer.l
	$(CC) -c -g lex.yy.c -o lex.yy.o
	$(CC) -c -g parser.tab.c -o parser.tab.o
	$(CC) -c -g utilities.c -o utilities.o
	$(CC) -c -g main.c -o main.o
	$(CC) -o parser.bin -g lex.yy.o parser.tab.o utilities.o main.o

clean:
	rm -fv *.bin *.yy* *.gv *.output *.tab.* lexer.c lexer.h parser.c parser.h *.o *.gch *~
