
all:
	flex lex.l
	gcc lex.yy.c -o lex
	./lex input.c -o example_output.txt