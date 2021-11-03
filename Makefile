INCLUDES=-I /usr/local/include/SDL2 -I ./include 
FLAGS=-g -D_GNU_SOURCE=1 -D_THREAD_SAFE
OBJECTS=./build/chip8_memory.o ./build/chip8_stack.o ./build/chip8_keyboard.o ./build/chip8.o ./build/chip8_screen.o
TESTS=./tests/chip8_keyboard_tests.o

all: ${OBJECTS} ${TESTS}
	echo "Running Tests"
	make test

	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./src/main.c -L /usr/local/lib -o ./bin/main -lSDL2main -lSDL2

./build/chip8_memory.o:src/chip8_memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8_memory.c -c -o ./build/chip8_memory.o
	
./build/chip8_stack.o:src/chip8_stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8_stack.c -c -o ./build/chip8_stack.o

./build/chip8_keyboard.o:src/chip8_keyboard.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8_keyboard.c -c -o ./build/chip8_keyboard.o

./build/chip8.o:src/chip8.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.o

./build/chip8_screen.o:src/chip8_screen.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8_screen.c -c -o ./build/chip8_screen.o

test:
	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./tests/chip8_keyboard_tests.c -o ./tests/chip8_keyboard_tests.o
	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./tests/chip8_stack_tests.c -o ./tests/chip8_stack_tests.o
	./tests/chip8_stack_tests.o
	./tests/chip8_keyboard_tests.o

clean:
	rm ./
