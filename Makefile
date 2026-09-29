CC = gcc

CFLAGS = -std=c11 -Wall -Wextra -pedantic -Iinclude

TARGET = wordle

OBJ = main.o wordle.o board.o input.o words.o

EXE = wordle.exe

ifeq ($(OS), Windows_NT)
    DELETE = del
else
    DELETE = rm -f
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS), Windows_NT)
	-del /Q $(OBJ) $(TARGET) $(EXE) 2>NUL
else
	$(DELETE) $(OBJ) $(TARGET)
endif

run: $(TARGET)
	./$(TARGET)

fix: clean run

.PHONY: all clean run fix