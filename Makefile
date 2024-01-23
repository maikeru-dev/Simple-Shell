
SOURCES := $(shell find src -name '*.c')

drun:
	make build && ./out/shell

run:
	gcc -Wall  $(SOURCES) -o ./out/shell && ./out/shell

build:
	gcc -Wall  $(SOURCES) -fsanitize=address -o ./out/shell

test: build
	./test.sh

clean:
	rm ./out/shell
