compile: chatserve chatclient

server: chatserve.cpp
		g++ -o chatserve chatserve.cpp

client: client.c
		gcc -o chatclient chatclient.c

clean:
	$(RM) *.o server client

