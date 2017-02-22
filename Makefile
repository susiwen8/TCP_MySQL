all : server client
.PHONY : all

CC = gcc
CFLAGS = -g -pthread -I /usr/include/mysql/ -lmysqlclient

SOURCE_SERVER  = server/server.c server/code.c server/rebound.c server/sendFile.c server/mysql.c
SOURCE_CLIENT  = client/client.c client/sendMessage.c client/download.c client/semaphore.c 
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
