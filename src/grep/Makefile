TARGET = s21_grep
CC = gcc
CFLAGS = -Werror -Wextra -Wall -std=c11 -g -c

all: $(TARGET) clean

$(TARGET): s21_grep.o
	$(CC)  s21_grep.o -o $(TARGET)

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep.o

test:
	bash grep_test.sh

clang:
	clang-format -i -style=google *.c *.h
	clang-format -n -style=google *.c *.h

leak:
	valgrind --leak-check=full --leak-resolution=med ./s21_grep -f t.txt t2.txt

clean:
	-rm -rf *.o

rebuild:
	make all

