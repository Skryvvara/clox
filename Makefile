all: main.c
		gcc main.c chunk.c memory.c debug.c -o ./dist/bin && ./dist/bin
