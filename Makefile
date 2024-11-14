
SRC = $(wildcard ./*.c) $(wildcard ./util/*.c) $(wildcard ./lexer/*.c)

all: $(SRC)
	gcc $(SRC)