# =========================
# Windows-friendly Makefile
# =========================

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -Iinclude
LDFLAGS = -lm
GCOVFLAGS = -fprofile-arcs -ftest-coverage

# Запускаем команды через cmd.exe (для корректной работы del/rmdir)
SHELL := cmd.exe
.SHELLFLAGS := /Q /C

# Windows-команды
RM = del /F /Q
RMDIR = rmdir /S /Q
NULL = NUL
EXE = .exe

# Пути и файлы
APP_SRC = src/main.c
LIB_SRC = src/mylib.c
TEST_SRC = test/test.c
LIB_OBJ = $(LIB_SRC:.c=.o)
APP_OBJ = $(APP_SRC:.c=.o)

LIB = lib\mylib.a
BIN_DIR = bin
TEST_BIN = test\run
TARGET = $(BIN_DIR)\app

# -------------------------------------
# Основная цель (по умолчанию) — all
# -------------------------------------
all: $(TARGET)$(EXE)

# Сборка основного приложения
$(TARGET)$(EXE): $(APP_OBJ) $(LIB)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) $(APP_OBJ) $(LIB) -o $(TARGET)$(EXE) $(LDFLAGS)
	@echo.
	@echo === Приложение собрано: $(TARGET)$(EXE) ===

# Сборка библиотеки
lib: $(LIB)

$(LIB): $(LIB_OBJ)
	@if not exist lib mkdir lib
	ar rcs $(LIB) $(LIB_OBJ)
	@echo === Библиотека создана: $(LIB) ===

# Правило для .o файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# -------------------------------------
# Сборка и запуск тестов
# -------------------------------------
test: $(LIB) $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB) -o $(TEST_BIN)$(EXE) $(LDFLAGS)
	@echo.
	@echo === Запуск тестов ===
	$(TEST_BIN)$(EXE)

# -------------------------------------
# Очистка проекта
# -------------------------------------
clean:
	@echo Удаление временных файлов...
	-$(RM) *.o 2>$(NULL)
	-$(RM) src\*.o 2>$(NULL)
	-$(RM) lib\*.a 2>$(NULL)
	-$(RM) $(TARGET)$(EXE) 2>$(NULL)
	-$(RM) $(TEST_BIN)$(EXE) 2>$(NULL)
	-$(RMDIR) $(BIN_DIR) 2>$(NULL)
	-$(RMDIR) report 2>$(NULL)
	@echo Очистка завершена.

