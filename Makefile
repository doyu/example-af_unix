all: stream dgram

stream: server_stream.c client_stream.c connection.h
	gcc server_stream.c -o server_stream.out -Wall
	gcc client_stream.c -o client_stream.out -Wall

dgram: server_dgram.c client_dgram.c connection.h
	gcc server_dgram.c -o server_dgram.out -Wall
	gcc client_dgram.c -o client_dgram.out -Wall

clean:
	rm -f *~ *.out


