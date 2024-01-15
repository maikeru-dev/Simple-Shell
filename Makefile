
SOURCES := $(shell find src -name '*.c')

build:
	gcc -Wall  $(SOURCES) -o ./out/shell

test: build
	./test.sh

clean:
	rm ./out/shell
