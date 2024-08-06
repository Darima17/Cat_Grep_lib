TARGET = s21_cat
CC = gcc
CFLAGS = -Werror -Wextra -Wall

all:
	make $(TARGET)

$(TARGET): s21_cat.o
	$(CC) $(CFLAGS)  s21_cat.o -o $(TARGET)

s21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) -c s21_cat.c -o s21_cat.o

test:
	bash t_cat.sh

clang:
	clang-format -i -style=google *.c
	clang-format -n -style=google *.c

leak:
	valgrind --leak-check=full --leak-resolution=med ./s21_cat -n bytes.txt

clean:
	-rm -rf *.o

rebuild:
	make clean
	make all