#general
INCLUDES=-I /usr/local/include/SDL2 -I ./include 
FLAGS=-g -D_GNU_SOURCE=1 -D_THREAD_SAFE
OBJECTS=./build/chip8_memory.o ./build/chip8_stack.o ./build/chip8_keyboard.o ./build/chip8.o ./build/chip8_screen.o ./build/toot.o
TESTS=./tests/chip8_keyboard_tests.o

#web
PROJECT=Chip8
HTML_OUT=./web/src/${PROJECT}.html
JS_OUT=./web/src/${PROJECT}.js 
WASM_OUT=./web/src/${PROJECT}.wasm
WASM_FILENAME=${PROJECT}.wasm
WASM_PUBLIC=./web/public/${PROJECT}.wasm
WASM_LOOKUP='wasmBinaryFile = locateFile(wasmBinaryFile)'
WASM_OBJECTS=./build/chip8_memory.html ./build/chip8_stack.html ./build/chip8_keyboard.html ./build/chip8.html ./build/chip8_screen.html ./build/toot.html
EMCC_OPTIONS=-s WASM=1 -s USE_SDL=2 -s -s MODULARIZE=1 -s ASYNCIFY -s EXPORT_NAME=${PROJECT}  -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s USE_ES6_IMPORT_META=0 -s EXPORT_ES6=1 -s LINKABLE=1 -s EXPORT_ALL=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall"]'


all: ${OBJECTS} 
	gcc -O3 ${FLAGS} ${INCLUDES} ${OBJECTS} ./src/main.c -L /usr/local/lib -o ./bin/Chip8 -lSDL2main -lSDL2

./build/chip8_memory.o:src/chip8_memory.c
	gcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8_memory.c -c -o ./build/chip8_memory.o
	
./build/chip8_stack.o:src/chip8_stack.c
	gcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8_stack.c -c -o ./build/chip8_stack.o

./build/chip8_keyboard.o:src/chip8_keyboard.c
	gcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8_keyboard.c -c -o ./build/chip8_keyboard.o

./build/chip8.o:src/chip8.c
	gcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.o

./build/chip8_screen.o:src/chip8_screen.c
	gcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8_screen.c -c -o ./build/chip8_screen.o

./build/toot.o:src/toot.c
	gcc -O3 ${FLAGS} ${INCLUDES} ./src/toot.c -c -o ./build/toot.o

wasm: ${WASM_OBJECTS}
	emcc ${FLAGS} ${INCLUDES} ${WASM_OBJECTS} ${EMCC_OPTIONS} ./src/main.c -o ${JS_OUT} --preload-file ./bin/c8games/ --use-preload-plugins
	cp ${WASM_OUT} ${WASM_PUBLIC}
	sed -i.old '1s;^;\/* eslint-disable *\/;' ${JS_OUT}
	sed -i.old "s|${WASM_FILENAME}|/${WASM_FILENAME}|" ${JS_OUT}
	sed -i.old "s|${WASM_LOOKUP}|// ${WASM_LOOKUP}|" ${JS_OUT}

./build/chip8_memory.html:src/chip8_memory.c
	emcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8_memory.c -c -o ./build/chip8_memory.html
	
./build/chip8_stack.html:src/chip8_stack.c
	emcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8_stack.c -c -o ./build/chip8_stack.html

./build/chip8_keyboard.html:src/chip8_keyboard.c
	emcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8_keyboard.c -c -o ./build/chip8_keyboard.html

./build/chip8.html:src/chip8.c
	emcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.html

./build/chip8_screen.html:src/chip8_screen.c
	emcc -O3 ${FLAGS} ${INCLUDES} ./src/chip8_screen.c -c -o ./build/chip8_screen.html

./build/toot.html:src/toot.c
	emcc -O3 ${FLAGS} ${INCLUDES} ./src/toot.c -c -o ./build/toot.html

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
