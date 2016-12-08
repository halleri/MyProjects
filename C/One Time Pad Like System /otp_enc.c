#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<sys/stat.h>
#include<fcntl.h>

#define LENGTH 512


//sends text file to server
void sendFile (char *filename, int sockfd, int filelength) {
	char buffer[LENGTH];
	FILE *f = fopen(filename, "r");
	bzero(buffer, LENGTH);
	int bytesSent;

	//read in file
	while((filelength = fread(buffer, sizeof(char), LENGTH, f)) > 0) {
		//send the file
		if((bytesSent = send(sockfd, buffer, filelength, 0)) < 0) {
			break;
		}
		bzero(buffer, LENGTH);
	}
	//if the bytes sent were 512 then server code ends
	if(bytesSent == LENGTH) {
		//send one 'null' packet to end server transmission
		send(sockfd, "0", 1, 0);
	}
	fclose(f);
	return;
}

int main(int argc, char *argv[]) {

	int clsockfd; //listen with this one
	int portno; //port number
	int n;
	int optval;
	struct sockaddr_in serv_addr;
	struct hostent *server; //host's information
	char buffer[LENGTH];
	bzero(buffer,sizeof(buffer));

	//make sure user entered the correct amount of arguments in the command line
	if (argc != 4) {
		printf("usage: otp_dec plaintext key port number\n");
		exit(0);
	}
	portno = atoi(argv[3]);
	//create socket
	clsockfd = socket(AF_INET, SOCK_STREAM, 0); 


	if (clsockfd < 0){
		error("Client: ERROR opening socket");
	} 
	//find server
	server = gethostbyname("localhost"); 

	if (server == NULL) {
		fprintf(stderr,"Client: ERROR, no such host\n");
		exit(0);
	}

	optval = 1;
	//allow port reuse
	setsockopt(clsockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	//clear the address 
	bzero((char *) &serv_addr, sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);


	//connect to the server
	if (connect(clsockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
		perror("Client: ERROR connecting");
		exit(1);
	}

	char auth[]="e";
	write(clsockfd, auth, sizeof(auth));
	read(clsockfd, buffer, sizeof(buffer));


	if (strcmp(buffer, "e") != 0) {
		fprintf(stderr,"otp_enc cannot use otp_dec_d\n");
		exit(2);
	}

	//assign the first argument 'text' to variable plainText
	//assign second argument 'key' to variable key
	int plainText = open(argv[1], O_RDONLY);
	int key = open(argv[2], O_RDONLY);

	//find the length of each one
	int textLength = lseek(plainText, 0, SEEK_END);
	int keyLength = lseek(key, 0, SEEK_END);

	//make sure the text length is not longer than the key length
	if(textLength > keyLength) {
		fprintf(stderr, "Key is too short");
		exit(1);
	}


	//this checks for bad characters	
	int plainfd = open(argv[1], 'r'); 
	while (read(plainfd, buffer, 1) != 0) {
		if(isalpha(buffer[0]) || isspace(buffer[0])){

		}else {
			fprintf(stderr,"%s contains invalid characters\n", argv[1]);
			exit(1);
		}

	}

	bzero(buffer, sizeof(buffer));
	//send text file
	sendFile(argv[1], clsockfd, textLength);
	//send key file
	sendFile(argv[2], clsockfd, keyLength);
	n = read(clsockfd,buffer,sizeof(buffer)-1);

	if (n < 0){
		perror("Client: ERROR reading from socket");
		exit(1);
	}

	printf("%s\n",buffer);
	close(clsockfd);
	return 0;

}

