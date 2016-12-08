#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>


#define MAX_CHAR 510


//chat function which accepts a socket, and the user's handle
void chat(int sockfd, char *handle) {
	//this will provide text for sending and receiving messages
	char buffer[MAX_CHAR];
	int quit = 0;
	int n;	
	//sets values in buffer to 0
	bzero(handle, 11);
	printf("What's your handle? ");
	//reads messages from stdin
	fgets(handle, 10, stdin);

	while(1) {
		printf("%s >> ", handle);
		bzero(buffer, MAX_CHAR);
		fgets(buffer, MAX_CHAR, stdin);
		//write to file descriptor
		n = write(sockfd, buffer, strlen(buffer));

		if(strstr(buffer, "\\quit") == NULL) {
		
	//		n = write(sockfd, buffer, strlen(buffer));
		}	
		else{

			quit == 1;
			close(sockfd);
			printf("Disconnected\n");
			return;
		}

		if(n < 0) {
			error("Error.");
		}

		bzero(buffer, MAX_CHAR);
		
		if (n < 0) {
			error("Error");
			printf("%s ", buffer);
		}
		//receive messages from server
		n = recv(sockfd,buffer,MAX_CHAR,0); 

		printf("Server >> %s",buffer);
	}
}

int main(int argc, char *argv[]) {
	int sockfd; //file descriptor that stores values returned by socket system
	char username[11];
	int portno; //stores port number
	int quit = 0;
	struct sockaddr_in serv_addr;
	struct hostent *server; 
	
	//user passes in a port number which the server will accept as an argument.
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if(sockfd < 0) {
		error("Error opening socket.");
	}
	//the port number will listen for a connection. the aoi function
	//converts a string to an integer
	portno = atoi(argv[2]); 
	//takes an argument and returns a pointer to a hostnet containing information about that host
	server = gethostbyname(argv[1]); 

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); 
	//serv_addr is a structure of type struct sockaddr_in. It contains code for the address family. It is necessary to convert this to network byte order by using function htons() which converts a port number in host byte order to a port number in network byte order
	serv_addr.sin_port = htons(portno);
	//establish connection with server
	connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
	printf("Connected To Server\n");
	//call chat function
	chat(sockfd, username);
	
	close(sockfd);

	return 0;
}
