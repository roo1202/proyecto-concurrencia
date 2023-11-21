CFLAGS = -Wall -pthread
CFILES := src/main.c src/list.c

bin/main: $(CFILES)
	gcc $(CFLAGS) -o $@ $?

