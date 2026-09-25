CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

TARGET = wordle

SRC = src/main.c src/wordle.c
OBJ = src/main.o src/wordle.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

src/main.o: src/main.c
	$(CC) $(CFLAGS) -Iinclude -c src/main.c -o src/main.o

src/wordle.o: src/wordle.c
	$(CC) $(CFLAGS) -Iinclude -c src/wordle.c -o src/wordle.o

clean:
	cmd /C "del /Q src\main.o src\wordle.o wordle.exe"

run: $(TARGET)
	$(TARGET).exe