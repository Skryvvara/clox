all: main.c
		gcc main.c chunk.c memory.c debug.c value.c vm.c -o ./dist/bin && ./dist/bin
