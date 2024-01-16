CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lws2_32

SERVER_SRC = mainserver.c
CLIENT_SRC = mainclient.c
COMMON_HEADERS = main.h

SERVER_TARGET = Server
CLIENT_TARGET = Client

.PHONY: all server client clean

all: server client

server: $(SERVER_TARGET)

client: $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_SRC) $(COMMON_HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRC) $(LIBS)

$(CLIENT_TARGET): $(CLIENT_SRC) $(COMMON_HEADERS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_SRC) $(LIBS)

clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)
