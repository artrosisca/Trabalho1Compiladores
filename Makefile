all: src/flex.l src/bison.y src/arvore.c src/arvore.h
	flex -o src/lex.yy.c src/flex.l
	bison -d -o src/y.tab.c src/bison.y
	gcc -o bin/compilador src/lex.yy.c src/y.tab.c src/arvore.c -lfl -lm
