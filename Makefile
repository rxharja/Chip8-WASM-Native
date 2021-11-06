INCLUDES=-I /usr/local/include/SDL2 -I ./include 
FLAGS=-g -D_GNU_SOURCE=1 -D_THREAD_SAFE
OBJECTS=./build/chip8_memory.o ./build/chip8_stack.o ./build/chip8_keyboard.o ./build/chip8.o ./build/chip8_screen.o ./build/toot.o
WASM_OBJECTS=./build/chip8_memory.html ./build/chip8_stack.html ./build/chip8_keyboard.html ./build/chip8.html ./build/chip8_screen.html ./build/toot.html
EMCC_OPTIONS=-s WASM=1 -s USE_SDL=2 
TESTS=./tests/chip8_keyboard_tests.o

all: ${OBJECTS} 
	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./src/main.c -L /usr/local/lib -o ./bin/Chip8 -lSDL2main -lSDL2

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

./build/toot.o:src/toot.c
	gcc ${FLAGS} ${INCLUDES} ./src/toot.c -c -o ./build/toot.o

web: ${WASM_OBJECTS}
	emcc ${FLAGS} ${INCLUDES} ${WASM_OBJECTS} ${EMCC_OPTIONS} ./src/main.c -L /usr/local/lib -o ./bin/Chip8.html --preload-file PONG2

./build/chip8_memory.html:src/chip8_memory.c
	emcc ${FLAGS} ${INCLUDES} ./src/chip8_memory.c -c -o ./build/chip8_memory.html
	
./build/chip8_stack.html:src/chip8_stack.c
	emcc ${FLAGS} ${INCLUDES} ./src/chip8_stack.c -c -o ./build/chip8_stack.html

./build/chip8_keyboard.html:src/chip8_keyboard.c
	emcc ${FLAGS} ${INCLUDES} ./src/chip8_keyboard.c -c -o ./build/chip8_keyboard.html

./build/chip8.html:src/chip8.c
	emcc ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.html

./build/chip8_screen.html:src/chip8_screen.c
	emcc ${FLAGS} ${INCLUDES} ./src/chip8_screen.c -c -o ./build/chip8_screen.html

./build/toot.html:src/toot.c
	emcc ${FLAGS} ${INCLUDES} ./src/toot.c -c -o ./build/toot.html

test:
	echo "Running Tests"
	make all
	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./tests/chip8_keyboard_tests.c -o ./tests/chip8_keyboard_tests.o
	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./tests/chip8_stack_tests.c -o ./tests/chip8_stack_tests.o
	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./tests/chip8_memory_tests.c -o ./tests/chip8_memory_tests.o
	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./tests/chip8_screen_tests.c -o ./tests/chip8_screen_tests.o
	gcc ${FLAGS} ${INCLUDES} ${OBJECTS} ./tests/chip8_instructions_tests.c -o ./tests/chip8_instructions_tests.o
	./tests/chip8_stack_tests.o
	./tests/chip8_memory_tests.o
	./tests/chip8_keyboard_tests.o
	./tests/chip8_screen_tests.o
	./tests/chip8_instructions_tests.o

clean:
	rm ./
