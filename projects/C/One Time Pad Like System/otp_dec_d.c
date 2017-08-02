#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define LENGTH 512

/*void error(const char *msg){
  perror(msg);
  exit(1);
  }*/

//converts a char to an int
int charToInt (char c){

	static const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

	int i;
	//loop throug the characters
	for (i = 0; i < 27; i++) {
		//if a character at a particular index matches what was input 
		if (c == chars[i]) {
			//return that index
			return i;
		}
	}

	return -1;
}

char intToChar(int i){

	static const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

	//if the number is not between 0 and 27
	if (i < 0 || 27 < i) {
		//invalid
		return 'a';
	}
	//otherwise return the character at that particular index
	return chars[i];
}

//decodes the message
void decrypt(char message[], char key[], int len){
	int i;
	char n;
	int textNum;
	int keyNum;
	int decryptText;
	len = (strlen(message)-1);	

	//loop through the message
	for (i = 0; i < len; i++) {

		//convert the text message to an int
		textNum = charToInt(message[i]);
		//convert key message to an int
		keyNum = charToInt(key[i]);

		decryptText = (textNum - keyNum) % 27;

		if(decryptText < 0) {
			decryptText += 27;
		}
		message[i] = intToChar(decryptText);

	}
	message[i] = '\0';
	return;

}

int main(int argc, char *argv[]) {

	int sockfd; //listen with this one
	int newsockfd; //connect with this one
	int portno; // port number
	int optval;
	socklen_t clilen; //stores address size of client
	char buffer[LENGTH];
	struct sockaddr_in serv_addr, cli_addr; //struct containing addr
	pid_t pid, sid;  //process id


	//make sure the correct number of arguments are entered in command line
	if (argc != 2) {
		fprintf(stderr,"usage %s <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}  

	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}
	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

	bzero((char *) &serv_addr, sizeof(serv_addr)); 
	//get port number
	portno = atoi(argv[1]); 
	//get host address
	serv_addr.sin_family = AF_INET;	
	//cannect to any client
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	//enter port number
	serv_addr.sin_port = htons(portno); 

	//bind address to socket
	if (bind(sockfd, (struct sockaddr *) &serv_addr, 
				sizeof(serv_addr)) < 0){ 
		perror("Decrypt Server: ERROR on binding");
		exit(1);
	}
	//listen for client call
	listen(sockfd,5); 

	//handles up to 5 connections
	while(1){   
		clilen = sizeof(cli_addr); 
		//accept the call
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); 
		if (newsockfd < 0){ 
			perror("ERROR on accept");
			exit(1);
		}

		//fork
		pid = fork();

		if (pid < 0){
			perror("ERROR forking process");
			exit(1);
		} 

		if (pid == 0){
			//child process. child will handle connection
			bzero(buffer, sizeof(buffer));
			int bytesLeft = sizeof(buffer);
			int bytesRead = 0;

			char *key;
			int newLine = 0;
			int i;

			//authenication
			read(newsockfd, buffer, sizeof(buffer)-1);
			if (strcmp(buffer, "dec") != 0) {
				char response[]  = "invalid";
				write(newsockfd, response, sizeof(response));
				exit(2);
			} 
			else {
				char response[] = "dec_d";
				write(newsockfd, response, sizeof(response));
			}
			bzero(buffer, sizeof(buffer));
			//pointer to buffer, so we can see where we are in the buffer
			char *ptrBuffer = buffer; 
			while (1){ 

				//read in the number of bytes and set it to bytesRead
				bytesRead = read(newsockfd, ptrBuffer, bytesLeft);

				//if the bytes are 0 then we're done reading
				if (bytesRead == 0){
					break;
				}
				//find new lines in text
				for (i=0; i < bytesRead; i++){ 
					//if we approach a new line
					if(buffer[i] == '\n'){
						//increment number of new lines
						++newLine;
						//when we reach one
						if (newLine == 1){ 
							//add up the buffer, index and 1 (because of the null terminator,) and set it to the key
							key = buffer+i+1;
						}
					}
				}
				//if we reach a second new line then end transmission
				if (newLine == 2){
					break;			 
				}
				//sum up the number of bytes read with the ptrBuffer and set it to ptrBuffer
				ptrBuffer += bytesRead;
				bytesLeft -= bytesRead;
			}
			char msg[LENGTH];
			bzero(msg, sizeof(msg));
			strncpy(msg, buffer, key-buffer); 
			decrypt(msg, key, strlen(msg));	

			write(newsockfd, msg, sizeof(msg));
		}
		close(newsockfd);
	}
	close(sockfd);


	return 0; 
}
