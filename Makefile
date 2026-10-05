CC = gcc

CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude

OBJ = main.o wordle.o board.o input.o words.o

ifeq ($(OS),Windows_NT)

TARGET = wordle.exe
RUN = .\wordle.exe
DELETE = del /Q

else

TARGET = wordle
RUN = ./wordle
DELETE = rm -f

endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	$(RUN)

clean:
ifeq ($(OS),Windows_NT)
	-$(DELETE) $(OBJ) wordle.exe wordle 2>NUL
else
	$(DELETE) $(OBJ) wordle wordle.exe
endif

.PHONY: all run clean