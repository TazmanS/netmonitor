CC=gcc
CFLAGS=-Wall -Wextra -Iinclude

SRC=$(wildcard src/*.c)

TARGET=build/netmonitor

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)