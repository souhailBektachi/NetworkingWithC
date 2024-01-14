# Makefile for the network_info program

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lws2_32 -liphlpapi

SRC = main.c
TARGET = network_info

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(TARGET)
