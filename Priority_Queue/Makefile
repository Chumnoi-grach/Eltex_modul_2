CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = priority_queue
SOURCES = main.c queue.c
HEADERS = queue.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run