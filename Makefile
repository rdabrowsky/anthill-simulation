CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pthread -D_XOPEN_SOURCE=700

.PHONY: all clean

all: bin/anthill bin/mother bin/ant

bin/anthill: src/main.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $<

bin/mother: src/modules/mother/main.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $<

bin/ant: src/modules/ant/main.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf bin/