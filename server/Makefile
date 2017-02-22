CC = gcc
CFLAGS = -g -I /usr/include/mysql/ -lmysqlclient

SOURCE  = server.c code.c rebound.c sendFile.c mysql.c
OBJECTS = $(SOURCE:.c = .o)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

server: $(OBJECTS) $(SOURCE)
		$(CC) $(OBJECTS) $(CFLAGS) -o ../bin/server

clean:
	rm -r ../bin/server
