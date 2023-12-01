CFLAGS = -Wall -pthread
CFILES := src/main.c src/list.c

bin/main: $(CFILES)
	mkdir -p bin
	gcc $(CFLAGS) -o $@ $?

