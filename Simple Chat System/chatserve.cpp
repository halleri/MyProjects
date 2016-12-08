#include <stdio.h> //allows input and output declarations
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> //socket definitions
#include <netinet/in.h> //internet domain addresses
#include <iostream>

#define MAX_CHAR 510

/*This function allows the server to send information to the client,
 * and receive information from the client.*/

void chat(int socket) {
	int n;
	char buffer[MAX_CHAR];
	int quit = 0;

	while(1) {
		//sets values in buffer to zero
		bzero(buffer,MAX_CHAR); 
		//receive message
	   	n = recv(socket,buffer,MAX_CHAR,0);
	   
		if(n == 0) {
	   		std::cout << "Client Disconnected" << std::endl;
	   		exit(0);
	   	}
	   	printf("%s", buffer);
	   	std::cout << "Server message: ";
		//read message from stdin
	   	fgets(buffer,MAX_CHAR,stdin);
		//write to file descriptor
		n = write(socket, buffer, strlen(buffer)); 
	   	if(strstr(buffer, "\\quit") == NULL) {
		 
		//	n = write(socket, buffer, strlen(buffer));
	   	}
		//if \quit is written then exit program
	   	else {
	   		quit == 1; 
	   		close(socket); 
	   		std::cout << "Server Disconnected" << std::endl;
	   		exit(0);
	   	}
		bzero(buffer, MAX_CHAR);
		n = write(socket, buffer, strlen(buffer));
   	}
   	return;
}

int main(int argc, char *argv[]) {
	int sockfd; //file descriptor that stores values returned by socket system
	int newsockfd;
	int portno; //stores port number 
	socklen_t clilen; //stores size of address of client
	int n; 
	struct sockaddr_in serv_addr, cli_addr; //struct containing internet address
	char username[11];
	if(argc < 2) { 
		std::cout << "You must enter a valid port number" << std::endl;
		exit(1); 
	}
	//create a new socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("Error opening socket.");
	}
	//sets all values to 0
	bzero((char *) &serv_addr, sizeof(serv_addr));
	//port number on which server will listen for connection
	//atoi converts from string to integer
	portno = atoi(argv[1]);
	//always set to this
	serv_addr.sin_family = AF_INET; 
	//set to this port
	serv_addr.sin_port = htons(portno); 
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	//binds program to address
	bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	//listen for a socket
	//5 is the max size permitted by most systems
	listen(sockfd,5); 

	std::cout << "Waiting for Client to connect..." << std::endl;

	clilen = sizeof(cli_addr);
		//the accept system call causes the process to block until a client connects to the server
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		chat(newsockfd);

    close(sockfd);


	return 0;
}
