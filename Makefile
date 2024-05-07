trab: lex.l fb-ex-2-1.y fb-ex-2-1.h
	bison -d fb-ex-2-1.y
	flex -o fb-ex-2-1.lex.c lex.l
	gcc -o $@ fb-ex-2-1.tab.c fb-ex-2-1.lex.c fb-ex-2-1-funcoes.c
	@echo Parser da Calculador com AST pronto ...
