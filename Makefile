all: ./src/main.c
	cd ./src && gcc -Wall main.c chunk.c memory.c debug.c value.c vm.c compiler.c scanner.c object.c table.c -o ../dist/clox && ../dist/clox
