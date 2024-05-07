all: src/flex.l src/bison.y src/arvore.c src/arvore.h
	mkdir bin
	flex -o src/lex.yy.c src/flex.l
	bison -d -o src/bison.tab.c src/bison.y
	gcc -o bin/compilador src/lex.yy.c src/bison.tab.c src/arvore.c -lfl -lm

