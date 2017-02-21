all : server client
.PHONY : all

CC = gcc
CFLAGS = -g -pthread -I /usr/include/mysql/ -lmysqlclient

SOURCE_SERVER  = server.c rebound.c sendFile.c mysql.c 
SOURCE_CLIENT  = client.c sendMessage.c download.c semaphore.c 
OBJECTS_SERVER = $(SOURCE_SERVER:.c = .o)
OBJECTS_CLIENT = $(SOURCE_CLIENT:.c = .o)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

server: $(OBJECTS_SERVER) $(SOURCE_SERVER)
		$(CC) $(OBJECTS_SERVER) $(CFLAGS) -o bin/server

client: $(OBJECTS_CLIENT) $(SOURCE_CLIENT)
		$(CC) $(OBJECTS_CLIENT) $(CFLAGS) -o bin/client

clean_all:
	rm -r bin/* file/*.*

clean_file:
	rm -r file/*.*

clean_bin:
	rm -r bin/*
