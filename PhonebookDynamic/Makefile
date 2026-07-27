CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -fPIC
INCLUDES = -Isrc -Isrc/Data -Isrc/UI -Isrc/Utiles

SHARED_FLAGS = -shared
LDFLAGS = -L$(LIB_DIR) -lphonebook



# CHECK_CFLAGS = $(shell pkg-config --cflags check)
# CHECK_LIBS = $(shell pkg-config --libs check)

SRC_DIR = src
DATA_DIR = $(SRC_DIR)/Data
UI_DIR = $(SRC_DIR)/UI
UTILES_DIR = $(SRC_DIR)/Utiles

LIB_DIR = lib
OBJ_DIR = build/obj
OBJ_LIB_DIR = build/obj/lib
BIN_DIR = build/bin

LIB_SOURCES = $(DATA_DIR)/contact.c $(DATA_DIR)/phonebook.c $(DATA_DIR)/validator.c
LIB_OBJECTS = $(OBJ_LIB_DIR)/contact_lib.o $(OBJ_LIB_DIR)/phonebook_lib.o $(OBJ_LIB_DIR)/validator_lib.o
LIB_SHARED = $(LIB_DIR)/libphonebook.so

MAIN_SOURCES = $(SRC_DIR)/main.c $(UI_DIR)/menu.c $(UI_DIR)/input.c $(UTILES_DIR)/utiles.c
MAIN_OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/UI/menu.o $(OBJ_DIR)/UI/input.o $(OBJ_DIR)/Utiles/utiles.o

TARGET = $(BIN_DIR)/phonebook
TARGET_SEED = $(BIN_DIR)/phonebook_seed

all: directories $(LIB_SHARED) $(TARGET_SEED)
	@echo "Сборка завершена!"
	@echo "Исполняемый файл: $(TARGET_SEED) \n"
	@echo "Доступные режимы:"
	@echo "  make              - Собрать (БД с данными)"
	@echo "  make with-data    - Собрать с исходными данными"
#	@echo "  make test         - Запустить unit-тесты"
	@echo "  make run          - Запустить программу"
	@echo "  make clean        - Очистить сборку"

# Библиотека
$(LIB_SHARED): $(LIB_OBJECTS)
	@mkdir -p $(LIB_DIR)
	$(CC) $(SHARED_FLAGS) -o $@ $^
	@echo "Статическая библиотека создана: $@"

# Компиляция файлов
# Для библы:
$(OBJ_LIB_DIR)/contact_lib.o: $(DATA_DIR)/contact.c $(DATA_DIR)/contact.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_LIB_DIR)/phonebook_lib.o: $(DATA_DIR)/phonebook.c $(DATA_DIR)/phonebook.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_LIB_DIR)/validator_lib.o: $(DATA_DIR)/validator.c $(DATA_DIR)/validator.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
# Остальное:

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# добавлен флаг -DWITH_SEED_DATA
$(OBJ_DIR)/main_seed.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(INCLUDES) -DWITH_SEED_DATA -c $< -o $@

# UI слой
$(OBJ_DIR)/UI/%.o: $(UI_DIR)/%.c $(UI_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Utiles слой
$(OBJ_DIR)/Utiles/%.o: $(UTILES_DIR)/%.c $(UTILES_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Линковка
$(TARGET): $(MAIN_OBJECTS) $(LIB_SHARED)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJECTS) $(LDFLAGS)
	@echo "Линковка завершена"

#$(TARGET_SEED): CFLAGS += -DWITH_SEED_DATA

$(TARGET_SEED): $(OBJ_DIR)/main_seed.o $(OBJ_DIR)/UI/menu.o $(OBJ_DIR)/UI/input.o $(OBJ_DIR)/Utiles/utiles.o $(LIB_SHARED)
#	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo " Линковка завершена (с данными)"

	

with-data: $(TARGET_SEED)
	@echo "Сборка с исходными данными завершена!"
	@echo "Исполняемый файл: $(TARGET_SEED)"
without-data: $(TARGET)
	@echo "Сборка с исходными данными завершена!"
	@echo "Исполняемый файл: $(TARGET)"


# ОСНОВНАЯ ПРОГРАММА

# КОМПИЛЯЦИЯ
directories: 
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/UI
	@mkdir -p $(OBJ_DIR)/Utiles
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_LIB_DIR)

#$(OBJ_DIR)/UI $(OBJ_DIR)/Utiles $(BIN_DIR)
#ЗАПУСК
# run: $(TARGET)
run:
	@echo "Запуск программы..."
	@LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(TARGET)

#	@LD_LIBRARY_PATH=$(shell pwd)/$(LIB_DIR):$$LD_LIBRARY_PATH ./$(TARGET)
#	./$(TARGET)

# run-test: $(TARGET_SEED)
run-test:
	@echo "Запуск программы (с тестовыми данными)..."
#	@LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(TARGET)
	./$(TARGET_SEED)





# ОЧИСТКА

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_LIB_DIR)
	@rm -rf $(BIN_DIR)
# 	@rm -rf $(TEST_OBJ_DIR)
# 	@rm -rf $(TEST_BIN_DIR)
	@rm -rf $(LIB_DIR)
	@echo " Очистка завершена"

clean_all: clean
	@rm -f $(TARGET) $(TARGET_SEED)
	@echo " Полная очистка завершена"


.PHONY: all clean clean_all run run-test directories with-data