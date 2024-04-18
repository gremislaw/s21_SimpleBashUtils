CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: build

build: s21_cat

s21_cat:
	$(CC) s21_cat.c $(CFLAGS) -o s21_cat

beauty:
	clang-format -i *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

clean:
	rm -f s21_cat

test: s21_cat
	chmod +x test.sh
	./test.sh

rebuild: clean build
