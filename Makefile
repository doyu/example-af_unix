all: server.c client.c connection.h
	gcc server.c -o server -Wall
	gcc client.c -o client -Wall

clean:
	rm -f *~ server client

