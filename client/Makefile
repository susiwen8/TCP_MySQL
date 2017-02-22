CC = gcc
CFLAGS = -g -pthread 

SOURCE  = client.c sendMessage.c download.c semaphore.c 
OBJECTS = $(SOURCE:.c = .o)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

client: $(OBJECTS) $(SOURCE)
		$(CC) $(CFLAGS) $(OBJECTS) -o ../bin/client

clean:
	rm -r ../bin/client
