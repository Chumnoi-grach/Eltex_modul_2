CC = gcc
CFLAGS = -Wall -Wextra -Wall -std=c99
TARGET = my_chmod

SOURCES = main.c permissions.c parser.c chmod_parser.c
HEADERS = permissions.h parser.h chmod_parser.h
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run