CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pthread -D_XOPEN_SOURCE=700
INCLUDES = -Isrc/common

# Pliki wspólne (common)
COMMON_SRC = $(wildcard src/common/*.c)

.PHONY: all clean

all: bin/anthill bin/mother bin/ant

bin/anthill: src/main.c $(COMMON_SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

bin/mother: src/modules/mother/main.c $(COMMON_SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

bin/ant: src/modules/ant/main.c $(COMMON_SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -rf bin/