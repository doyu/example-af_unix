all: stream

stream: server_stream.c client_stream.c connection.h
	gcc server_stream.c -o server -Wall
	gcc client_stream.c -o client -Wall

clean:
	rm -f *~ server client

