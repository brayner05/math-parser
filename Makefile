
CC = gcc
CFLAGS = -g -Wall

EXEC = calculate

BIN = bin

SRC = $(wildcard ./*.c) $(wildcard ./util/*.c) $(wildcard ./lexer/*.c)

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN)/$(EXEC) $(SRC)