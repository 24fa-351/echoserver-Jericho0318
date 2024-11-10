echoserver: echoserver.c
		gcc -lpthread -o echoserver echoserver.c

clean:
	rm echoserver