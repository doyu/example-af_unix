all: stream

stream: server_stream.c client_stream.c connection.h
	gcc server_stream.c -o server -Wall
	gcc client_stream.c -o client -Wall

dgram: server_dgram.c client_dgram.c connection.h
	gcc server_dgram.c -o server -Wall
	gcc client_dgram.c -o client -Wall

clean:
	rm -f *~ server client

