
SOURCES := $(shell find src -name '*.c')

build:
	mkdir ./out
	gcc -Wall $(SOURCES) -o ./out/shell

test: build
	./test.sh

clean:
	rm ./out/shell
