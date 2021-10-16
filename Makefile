INCLUDES=-I /usr/local/include/SDL 
FLAGS=-g -D_GNU_SOURCE=1 -D_THREAD_SAFE
all:
	gcc ${FLAGS} ${INCLUDES} ./src/main.c -L /usr/local/lib -o ./bin/main -lSDL2main -lSDL2
