CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wno-unused-value -Wno-unused-parameter -fPIC
LDFLAGS = -ldl
SHARED_FLAGS = -shared

# Директории
SRC_DIR = src
OPS_DIR = operations
LIB_DIR = lib
BIN_DIR = bin

# Основная программа
TARGET = $(BIN_DIR)/calculator

# Библиотеки операций
OPERATIONS = summ sub mul divide power ost
LIBRARIES = $(foreach op,$(OPERATIONS),$(LIB_DIR)/lib$(op).so)

all: directories $(TARGET) $(LIBRARIES)

# Основная программа
$(TARGET): $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Каждая библиотека
$(LIB_DIR)/lib%.so: $(OPS_DIR)/%.c
	$(CC) $(SHARED_FLAGS) -o $@ $^ -lm

directories:
	@mkdir -p $(BIN_DIR) $(LIB_DIR)

clean:
	rm -rf $(BIN_DIR) $(LIB_DIR)

run: directories $(TARGET) $(LIBRARIES)
	./$(TARGET)

run-test: directories $(TARGET)
	./$(TARGET)

.PHONY: all clean directories run