all:
	gcc -Wall -Wextra -I ./include ./deps/ReMem/ReMem.c ./deps/ReMem/arena/arena.c ./src/*.c ./marrow-lang.c -o marrow-lang

clean:
	rm marrow-lang
