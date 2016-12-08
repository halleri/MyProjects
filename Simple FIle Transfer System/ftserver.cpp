#include <iostream>
#include <dirent.h>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>

#define BACKLOG 10
#define MAX_CHARS 1024	
//handles signals for child processes
void sigchld_handler(int s) {
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

//IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
    	return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//this function was modfiied from 
//www.informit.com/guides/content.aspx?g=cplusplus&seqNum=245
void send_dir(int sockfd, char *host, int port) {
    	DIR *pdir;
   	struct dirent *pent;
	char service[20];
	struct sockaddr_in sd;	
	
	//gets the client name
	getnameinfo((struct sockaddr *)&sd, sizeof sd, host, sizeof host, service, sizeof service, 0);
    
	//open the current directory
	pdir=opendir("."); //"." refers to the current dir

	//if the current directory doesn't exist
    	if (!pdir){
        	printf ("opendir() failure; terminating");
        	exit(1);
    	}
    	errno=0;
	printf("List directory requested on port %d\n", port);
	printf("Sending directory contents to %s:%d\n",host, port);
	//while the directory exists
    	while ((pent=readdir(pdir))){
		//send its contents to the client
        	send(sockfd, pent->d_name, strlen(pent->d_name), 0);
        	send(sockfd, "\n", 1, 0);
	}
    	if (errno){
        	printf ("readdir() failure; terminating");
        	exit(1);
    	}
    	closedir(pdir);
}

//code based off of stackoverflow.com/questions/15627492/c-send-file-and-text-via-socket
int send_file(int new_fd, const char* filename, int port, char *host) {

   	int listenfd = 0;
    	int connfd = 0;
    	struct sockaddr_in serv_addr;
    	char sendBuff[1025];
    	int numrv;
    	int n;
	char service[20];

//	beej.us/guide/bgnet/output/html/multipage/getnameinfoman.html	
	getnameinfo((struct sockaddr *)&serv_addr, sizeof serv_addr, host, sizeof host, service, sizeof service, 0);

	//get the socket file descriptor
    	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	char buf[1024];

	//listen for remote connection
    	if(listen(listenfd, 10) == -1) {
        	printf("Failed to listen\n");
        	return -1;

	}
	
	//open the file
        FILE *fp = fopen(filename,"rb");
	//if the file doesn't exist
        if(fp==NULL) {
        
		 printf("File not found. Sending error message to %s:%d\n", host, port);		//send the client a message
		send(new_fd, "FILE NOT FOUND", strlen("FILE NOT FOUND"), 0);
		
		exit(1);				
        
   	 }   else {
		printf("Sending file to %s:%i\n", host, port);	
	}
        close(connfd);
	fclose(fp);
	sleep(1);
    
}

//beej.us/guide/bgnet/output/html/singlepage/bgnet.html#connect
int con(char s[INET6_ADDRSTRLEN], int port, const char* file) {
	int sockfd;
	struct sockaddr_in servaddr;

	char host[MAX_CHARS];
	char service[20];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	inet_pton(AF_INET, s, &servaddr.sin_addr);
	
	getnameinfo((struct sockaddr *) &servaddr, sizeof servaddr, host, sizeof host, service, sizeof service, 0);	

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	if (file == NULL) {
		send_dir(sockfd, host, port);
	} else if (send_file(sockfd, file, port, host) == -1) {

		close(sockfd);
		return -1;
	} 
	close(sockfd);
	return 0;
}

int instruct(int new_fd, char s[INET6_ADDRSTRLEN], char *host) {
	char buffer[MAX_CHARS];
	int n, port;
	char* file;
	char service[20];
	struct sockaddr_in servaddr;
	getnameinfo((struct sockaddr *)&servaddr, sizeof servaddr, host, sizeof host, service, sizeof service, 0);

	//receive data from client
	n = recv(new_fd, buffer, MAX_CHARS, 0);
	buffer[n] = 0;
	//if the port is entered then
	if (strncmp(buffer, "port", sizeof(port)) == 0) {
		//set the buffer to 0
		buffer[sizeof(buffer)] = 0;
		//get only the number for the port	
		port = atoi(&buffer[sizeof(port)]);
	} 


	//if directory is selected then
	//the '11' is based on how many characters are in 'port' and the port number, assuming 
	//there are five characters in port number.
	if (strncmp(&buffer[11], "dir", sizeof("dir")-1) == 0) {
		//get teh directory
		con(s, port, NULL);
	//if a file is requested then
	} else if (strncmp(&buffer[11], "file", sizeof("file")-1) == 0) {
		//put the contens of the buffer into the variable file
		//the '16' is based on there are 16 characters preceding
		//thename of the file - ex port 30021 file
		//it also assumes the port number will have 5 characters
		file = &buffer[16];
		printf("File '%s' requested on port %i\n", file, port);
		(con(s, port, file) == -1);
		return 0;
	}

}

//kthe majority of this code was borrwed from Beej's Guide
//beej.us/guide/bgnet/output/html/singlepage/bgnet.html#simpleserver
int main(int argc, char *argv[]) {
	
	int sockfd;
	char* port;

	struct addrinfo hints, *servinfo, *p;
	struct sigaction sa;
	int yes=1;
	int rv;
	int new_fd;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];
	char send_me[MAX_CHARS];
	
	if (argc > 1) {
		port = argv[1];
	} else {
		printf("usage: ./server port\n");
		return -1;
	}


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; 

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	
	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); 

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; 
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	printf("Server open on %s\n", port);

	char host[1024];
	char service[20];
	
	while(1) { 
		
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
			
		getnameinfo((struct sockaddr *)&their_addr, sizeof their_addr, host, sizeof host, service, sizeof service, 0);
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
		printf("Connection from %s\n", host);

		if (!fork()) { 
			close(sockfd); 
			instruct(new_fd,s, host);
			
			close(new_fd);
			exit(0);
		}
		close(new_fd);  
	}

	return 0;
}
