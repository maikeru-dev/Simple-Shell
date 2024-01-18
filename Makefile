
SOURCES := $(shell find src -name '*.c')

build:
	gcc -Wall  $(SOURCES) -fsanitize=address -o ./out/shell

test: build
	./test.sh

clean:
	rm ./out/shell
