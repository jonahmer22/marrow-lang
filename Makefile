all:
	gcc -Wall -Wextra -I ./include ./deps/ReMem/ReMem.c ./deps/ReMem/arena/arena.c ./src/*.c -o marrow-lang

clean:
	rm marrow-lang

test:
	python3 -m venv env
	source ./env/bin/activate
	pip3 install pytest

clean-test:
	rm -rf ./env/
