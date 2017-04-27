#Sample Makefile. You can make changes to this file according to your need
# The executable must be named proxy

CC = gcc
CFLAGS = -Wall -g 
LDFLAGS = -pthread

OBJS = chat-room-server.o csapp.o

all: chat-room-server

chat-room-server: $(OBJS)

csapp.o: csapp.c
	$(CC) $(CFLAGS) -c csapp.c $(LDFLAGS)

chat-room-server.o: chat-room-server.c
	$(CC) $(CFLAGS) -c chat-room-server.c

clean:
	rm -f *~ *.o chat-room-server

