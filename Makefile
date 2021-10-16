INCLUDES=-I ./include
FLAGS=-g
all:
	gcc ${FLAGS} ${INCLUDES} ./src/main.c -o ./bin/main -lSDL2main -lSDL2
