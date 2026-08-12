CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

TARGET = sudoku
SOURCES = main.c sudoku.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = sudoku.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean