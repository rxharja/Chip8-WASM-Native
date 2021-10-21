INCLUDES=-I /usr/local/include/SDL -I ./include 
FLAGS=-g -D_GNU_SOURCE=1 -D_THREAD_SAFE

OBJECTS=./build/chip8_memory.o
all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./src/main.c -L /usr/local/lib -o ./bin/main -lSDL2main -lSDL2

./build/chip8_memory.o:src/chip8_memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8_memory.c -c -o ./build/chip8_memory.o

clean:
	rm ./build/*