CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -lm
CPPFLAGS = -I.
TARGET = main
SRC = main.c $(wildcard src/*.c)
OBJ = *.o

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run