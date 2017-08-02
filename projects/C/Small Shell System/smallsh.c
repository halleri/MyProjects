#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define MAX_CHARS 513

int sigNum = 0;
//for foreground process
pid_t fgpid = INT_MAX;        

//prints out either an exit value or terminated by signal value
//source: modified from leture 9
void currStatus(int status) {
	int exitStatus = WEXITSTATUS(status);
	if (WIFEXITED(status)) {
        	printf("exit value %i\n", exitStatus);
    	} else {
        	printf("terminated by signal %i\n", status);
   	 }
}


void sigHandler() {
	//when a foreground process is running and an interrupt
	//signal occurs
	if (fgpid != INT_MAX) {
		//terminate foreground process
     	   	kill(fgpid, SIGKILL);
		//set sigNum to 2
 		sigNum = 2;  
    	}  
	//ignore for background processes
    	return;
}
int main() {
	char input[MAX_CHARS];
	char* args[MAX_CHARS];
	char* token;
	char* inputFile = NULL;  
	char* outputFile = NULL;  
	int fd = -1;          
	int fd2 = -1;         
	int bg;     
	int status = 0;
	int pid;
	int numberOfArgs;

	//sigaction struct 
	struct sigaction act;
	act.sa_handler = sigHandler;
	act.sa_flags = SA_RESTART;
	act.sa_flags = 0;
	sigfillset(&(act.sa_mask));
	sigaction(SIGINT, &act, NULL); 

	while (1) {
		//background?
		bg = 0;
		printf(": ");       
		fflush(stdout);     

		//read in a line
		if (fgets(input, MAX_CHARS, stdin) == NULL) {
			return 0;
		}
		//split line into tokens
		numberOfArgs = 0;
		token = strtok(input, " \n");
		while (token != NULL) {
			if (strcmp(token, "<") == 0) {
				//input file
				//get file name
				token = strtok(NULL, " \n");
				//copy fd2 to variable
				inputFile = strdup(token);
				token = strtok(NULL, " \n");
			} else if (strcmp(token, ">") == 0) {
				//output file
				//get file name
				token = strtok(NULL, " \n");
				//copy file to variable
				outputFile = strdup(token);
				//get next argument
				token = strtok(NULL, " \n");
			} else if (strcmp(token, "&") == 0) {
				//now in background
				bg = 1;
				break;
			} else {
				//store command in array
				args[numberOfArgs] = strdup(token);
				token = strtok(NULL, " \n");
				//increment arguments
				numberOfArgs++;
			}
		}
		args[numberOfArgs] = NULL;
		//is it a comment or null?
		if (args[0] == NULL || *(args[0]) == '#') {
			; //do nothing if comment or null
		} else if (strcmp(args[0], "status") == 0) {
			currStatus(status);
		} else if (strcmp(args[0], "cd") == 0) {
			if (args[1] == NULL) {
				//go to home directory
				chdir( getenv("HOME") );
			} else {
				//or go to different directory
				chdir(args[1]);
			}
		} else if (strcmp(args[0], "exit") == 0) {
			//when 'exit' is entered, safely exit
			exit(0);
		} else {
			//fork command
			pid = fork();
			
			switch (pid) {
			//child process
			case 0: 
			//if not in background    
			if (!bg) {
				act.sa_handler = SIG_DFL;
				sigaction(SIGINT, &act, NULL);
			}
		if (inputFile != NULL) {
			//if file name is given, only open read only file
			fd = open(inputFile, O_RDONLY);
			if (fd == -1) {
				//invalid file name
				printf("cannot open %s for input\n", inputFile);
				fflush(stdout);
				exit(1);
			}
			if (dup2(fd, 0) == -1) {
				//redirect input
				perror("dup2");
				exit(1);
			}
			//close file stream
			close(fd);
		} else if (bg) {
			//if the process is in hte background
			//redirect input to dev/null
			fd = open("/dev/null", O_RDONLY);
			if (fd == -1) {
				perror("open");
				exit(1);
			}
			if (dup2(fd, 0) == -1) {
				perror("dup2");
				exit(1);
			}
		}
		//if file name is given
		if (outputFile != NULL) {
			//open file
			fd2 = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0744);
			if (fd2 == -1) {
				printf("cannot open %s for output\n", outputFile);
				fflush(stdout);
				exit(1);
			}
			if (dup2(fd2, 1) == -1) {
				perror("dup2");
				exit(1);
			}
			close(fd2);
		}
	//execute command
	if (execvp(args[0], args)) {
		printf("%s: no such file or directory\n", args[0]);
		fflush(stdout);
		exit(1);
	}
	break;
	//fork error if pid < 0
	case -1:   
		perror("fork");
		status = 1;
		break;
	//parent process
	default:    
		if (!bg) {
			//reset sigNum
			sigNum = 0;
			//assign pid to fgpid
			//for access in sig handlers
			fgpid = pid;
			//set interrupt handler for foregrond process
			sigaction(SIGINT, &act, NULL);
			//wait for child process
			fgpid = waitpid(fgpid, &status, 0);
			fgpid = INT_MAX;
			waitpid(pid, &status, 0);

			if(sigNum != 0) {
				printf("terminated by signal %d\n", sigNum);
			}	
			} else {
				printf("background pid is %i\n", pid);
				break;
			}
		}
	}
	//emtpy argument array for reuse
	int i;
	for (i = 0; i < numberOfArgs; i++) {
		args[i] = NULL;
	}
	
	//free up file names
	inputFile = NULL;
	outputFile = NULL;
	pid = waitpid(-1, &status, WNOHANG);

	//background process done?
	//check if anything is alive
	while (pid > 0) {
		printf("background pid %i is done: ", pid);
		currStatus(status);
		pid = waitpid(-1, &status, WNOHANG);
	}
}

return 0;

}

