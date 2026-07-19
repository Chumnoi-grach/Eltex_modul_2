CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -Isrc -Isrc/Data -Isrc/UI -Isrc/Utiles



CHECK_CFLAGS = $(shell pkg-config --cflags check)
CHECK_LIBS = $(shell pkg-config --libs check)


SRC_DIR = src
DATA_DIR = $(SRC_DIR)/Data
UI_DIR = $(SRC_DIR)/UI
UTILES_DIR = $(SRC_DIR)/Utiles

OBJ_DIR = build/obj
BIN_DIR = build/bin
TEST_OBJ_DIR = build/test_obj
TEST_BIN_DIR = build/test_bin

# SOURCES = $(SRC_DIR)/main.c \
#           $(DATA_DIR)/contact.c \
#           $(DATA_DIR)/phonebook.c \
#           $(UI_DIR)/menu.c \
#           $(UI_DIR)/input.c \
#           $(UTILES_DIR)/utiles.c \
#           $(DATA_DIR)/validator.c

# OBJECTS = $(OBJ_DIR)/main.o \
#           $(OBJ_DIR)/Data/contact.o \
#           $(OBJ_DIR)/Data/phonebook.o \
#           $(OBJ_DIR)/UI/menu.o \
#           $(OBJ_DIR)/UI/input.o \
#           $(OBJ_DIR)/Utiles/utiles.o \
#           $(OBJ_DIR)/Data/validator.o

COMMON_SOURCES = $(DATA_DIR)/contact.c \
                 $(DATA_DIR)/phonebook.c \
                 $(UTILES_DIR)/utiles.c \
                 $(DATA_DIR)/validator.c

COMMON_OBJECTS = $(OBJ_DIR)/Data/contact.o \
                 $(OBJ_DIR)/Data/phonebook.o \
                 $(OBJ_DIR)/Utiles/utiles.o \
                 $(OBJ_DIR)/Data/validator.o

TEST_SOURCES = $(wildcard tests/test_*.c) tests/test_runner.c
TEST_OBJECTS = $(patsubst tests/%.c, $(TEST_OBJ_DIR)/%.o, $(TEST_SOURCES))

TEST_TARGET = $(TEST_BIN_DIR)/test_runner





MAIN_SOURCES = $(SRC_DIR)/main.c $(UI_DIR)/menu.c $(UI_DIR)/input.c
MAIN_OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/UI/menu.o $(OBJ_DIR)/UI/input.o

TARGET = $(BIN_DIR)/phonebook
TARGET_SEED = $(BIN_DIR)/phonebook_seed




all: directories $(TARGET)
	@echo "Сборка завершена!"
	@echo "Исполняемый файл: $(TARGET) \n"
	@echo "Доступные режимы:"
	@echo "  make              - Собрать (пустая БД)"
	@echo "  make with-data    - Собрать с исходными данными"
	@echo "  make test         - Запустить unit-тесты"
	@echo "  make run          - Запустить программу"
	@echo "  make clean        - Очистить сборку"

# TESTS

test: directories $(TEST_TARGET)
	@echo "Запуск тестов..."
	@./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(COMMON_OBJECTS)
	@mkdir -p $(TEST_BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(CHECK_LIBS)
	@echo "Тесты слинкованы"

$(TEST_OBJ_DIR)/%.o: tests/%.c
	@mkdir -p $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(CHECK_CFLAGS) -c $< -o $@



# ОСНОВНАЯ ПРОГРАММА

$(TARGET): $(MAIN_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Линковка завершена (пустая БД)"

$(TARGET_SEED): CFLAGS += -DWITH_SEED_DATA
$(TARGET_SEED): $(OBJ_DIR)/main_seed.o $(COMMON_OBJECTS) $(OBJ_DIR)/UI/menu.o $(OBJ_DIR)/UI/input.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Линковка завершена (с данными)"

with-data: $(TARGET_SEED)
	@echo "Сборка с исходными данными завершена!"
	@echo "Исполняемый файл: $(TARGET_SEED)"




# $(TARGET): $(OBJECTS)
# 	$(CC) $(CFLAGS) -o $@ $^
# 	@echo "Линковка завершена (пустая БД)"

# with-data: CFLAGS += -DWITH_SEED_DATA
# with-data: directories $(OBJECTS)
# 	$(CC) $(CFLAGS) -o $(TARGET_SEED) $(filter %.o, $^)
# 	@echo "Сборка с исходными данными завершена!"
# 	@echo "Исполняемый файл: $(TARGET_SEED)"

# both: directories $(TARGET) with-data
# 	@echo ""
# 	@echo "Оба варианта собраны!"
# 	@echo "Пустая БД:    $(TARGET)"
# 	@echo "Тестовые данные: $(TARGET_SEED)"


# КОМПИЛЯЦИЯ
directories:
	@mkdir -p $(OBJ_DIR)/Data
	@mkdir -p $(OBJ_DIR)/UI
	@mkdir -p $(OBJ_DIR)/Utiles
	@mkdir -p $(BIN_DIR)

# $(TARGET): $(OBJECTS)
# 	$(CC) $(CFLAGS) -o $@ $^
# 	@echo "Линковка завершена"

# main.c для пустой БД
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# main.c для БД с данными
$(OBJ_DIR)/main_seed.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(INCLUDES) -DWITH_SEED_DATA -c $< -o $@

# Data слой
$(OBJ_DIR)/Data/%.o: $(DATA_DIR)/%.c $(DATA_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# UI слой
$(OBJ_DIR)/UI/%.o: $(UI_DIR)/%.c $(UI_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Utiles слой
$(OBJ_DIR)/Utiles/%.o: $(UTILES_DIR)/%.c $(UTILES_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@




#ЗАПУСК
run: $(TARGET)
	@echo "Запуск программы..."
	./$(TARGET)

run-test: $(TARGET_SEED)
	@echo "Запуск программы (с тестовыми данными)..."
	./$(TARGET_SEED)





# ОЧИСТКА

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)
	@rm -rf $(TEST_OBJ_DIR)
	@rm -rf $(TEST_BIN_DIR)
	@echo "🧹 Очистка завершена"

clean_all: clean
	@rm -f $(TARGET) $(TARGET_SEED)
	@echo "🧹 Полная очистка завершена"



# # ОТЛАДКА
# debug: CFLAGS += -DDEBUG -O0 -g3 -fsanitize=address
# debug: clean all

# debug-test: CFLAGS += -DDEBUG -O0 -g3 -fsanitize=address
# debug-test: clean test

.PHONY: all clean clean_all run run-test directories with-data test