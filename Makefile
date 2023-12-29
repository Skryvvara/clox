all: main.c
		gcc -Wall main.c chunk.c memory.c debug.c value.c vm.c compiler.c scanner.c object.c -o ./dist/bin && ./dist/bin
