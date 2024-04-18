CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: build

build: s21_grep

s21_grep:
	$(CC) s21_grep.c $(CFLAGS) -o s21_grep

beauty:
	clang-format -i *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

clean:
	rm -f s21_grep

test:
	chmod +x test.sh
	./test.sh

rebuild: clean build