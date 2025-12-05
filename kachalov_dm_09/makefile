CC = gcc                           # используем компилятор gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -Iinclude   # флаги: все предупреждения, ошибки, строгий стандарт C11, подключаем папку include
GCOVFLAGS = -fprofile-arcs -ftest-coverage          # флаги для профилирования покрытия (gcov)

APP_SRC = src/main.c               # исходный файл с функцией main
LIB_SRC = src/mylib.c              # исходный файл с реализацией библиотеки

APP_OBJ = $(APP_SRC:.c=.o)         # объектный файл для main.c
LIB_OBJ = $(LIB_SRC:.c=.o)         # объектный файл для mylib.c

TEST_SRC = test/test.c             # тестовый исходник
TEST_BIN = test/run                # исполняемый файл тестов

LIB = mylib.a                      # статическая библиотека
TARGET = bin/app                   # итоговый исполняемый файл программы

all: $(TARGET) test                # цель по умолчанию: собрать приложение и тесты

lib: $(LIB)                        # цель для сборки только библиотеки

$(LIB): $(LIB_OBJ)                 # правило сборки библиотеки только из объектника mylib.o
	ar rcs $@ $^                   # создаём архив mylib.a из объектных файлов библиотеки

$(TARGET): $(APP_OBJ) $(LIB)       # правило сборки основного приложения
	mkdir -p bin                   # создаём папку bin, если её нет
	$(CC) $(CFLAGS) $(APP_OBJ) $(LIB) -o $@   # компилируем и линкуем main.o с библиотекой

%.o: %.c                           # шаблонное правило: как собирать .o из .c
	$(CC) $(CFLAGS) -c $< -o $@    # компиляция .c в .o

test: $(LIB) $(TEST_SRC)           # цель для сборки и запуска тестов
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB) -o $(TEST_BIN)  # компиляция тестов с библиотекой (без check!)
	./$(TEST_BIN)                  # запуск собранного теста

gcov_report: CFLAGS += $(GCOVFLAGS)         # добавляем флаги gcov к CFLAGS
gcov_report: clean $(APP_SRC) $(LIB_SRC) $(TEST_SRC)  # цель gcov_report зависит от очистки и исходников
	$(CC) $(CFLAGS) $(APP_SRC) $(LIB_SRC) $(TEST_SRC) -o $(TEST_BIN) # сборка с флагами gcov
	./$(TEST_BIN)                  # запуск тестов для сбора покрытия
	lcov -t "mylib" -o coverage.info -c -d .   # снимаем покрытие в файл coverage.info
	lcov --remove coverage.info "*/test/*" -o coverage.info  # убираем покрытие тестовых файлов
	genhtml -o report coverage.info            # генерируем html-отчёт в папку report

leaks: test                        # цель проверки утечек памяти
	leaks --atExit -- ./test/run   # macOS утилита leaks для поиска утечек

clean:                             # очистка проекта
	rm -f $(APP_OBJ) $(LIB_OBJ) $(LIB) *.gcno *.gcda *.gcov coverage.info  # удаляем объектники, lib и gcov-файлы
	rm -f $(TARGET) $(TEST_BIN)     # удаляем исполняемые файлы
	rm -rf report bin               # удаляем папки report и bin

.PHONY: all lib clean test gcov_report leaks  # указываем, что это не файлы, а «фальшивые» цели