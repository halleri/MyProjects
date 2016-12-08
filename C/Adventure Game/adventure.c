#include <stdio.h> 
#include <stdlib.h>
#include <time.h> 
#include <unistd.h> 
#include <sys/stat.h> 
#include <string.h> 


#define MAX_ROOMS 7
#define MAX_ROOM_NAMES 10
#define MAX_CONNECTIONS 6
#define MIN_CONNECTIONS 3

//Holds all rooom information
struct Room {
	char name[20];
	char type[20];
	int connections[MAX_CONNECTIONS];
	int numConnections;
};
struct Room rooms[MAX_ROOMS];  //an array to hold struct information

//the 10 rooms to use
char *roomNames[] = {"Winterfell", "Eyrie", "Harrenhal", "Highgarden", "Sunspear", "Dragonstone", "Lys", "Pyke", "Westeros", "Dorne"};

//variables that hold current and end rooms
int curr, end;


//function that takes an array, and swaps two integers within it
void swap(int *arr, int A, int B){
	int tmp;
	tmp = arr[A];
	arr[A] = arr[B];
	arr[B] = tmp;
	return;
}

//function that generates a random room 
char* randomRm(char **names, int numNames){
	int randNum = rand() % numNames;	
	return names[numNames - 1]; // the -1 is to prevent a core dump
}


//function that returns an index of a given room
int find(char* name){
	int i;
	for (i=0; i<MAX_ROOMS; i++){
		if (strcmp(name, rooms[i].name) == 0){
			return i;		
		}
	}
	return -1;	
}
//function that creates the files
void createFile(struct Room room, int roomNum) {
	FILE *ptrFile;
	int i;
	char roomName[10];
	sprintf(roomName, "%d" , roomNum);
	ptrFile = fopen(roomName, "w");
	if (ptrFile == NULL) {
		perror("ERROR!");
		exit(1);
	}
	fprintf(ptrFile, "ROOM NAME: %s\n", room.name);
	for (i = 0; i < room.numConnections; i++) {
		fprintf(ptrFile, "CONNECTION %d: ", i+1);
		fprintf(ptrFile, "%s\n", rooms[room.connections[i]].name);
	}
	fprintf(ptrFile, "ROOM TYPE: %s\n", room.type);
	fclose(ptrFile);
	return;

}

//function that assigns a random room to each room type
void getRoom() {

	srand(time(NULL));
	int i, randRoom;
	for (i=0; i<MAX_ROOMS; i++){
		strcpy(rooms[i].name, randomRm(roomNames, MAX_ROOM_NAMES - i));

		strcpy(rooms[i].type, "MID_ROOM");
	}
	//assign a random number to randRoom		
	randRoom = rand() % MAX_ROOMS;
	strcpy(rooms[randRoom].type,"START_ROOM");
	randRoom = rand() %  (MAX_ROOMS - 1);
	strcpy(rooms[randRoom].type, "END_ROOM");

	for (i=0; i<MAX_ROOMS; i++){
		if (strcmp(rooms[i].type, "START_ROOM")== 0){
			curr = i; 
		}
		if (strcmp(rooms[i].type, "END_ROOM")== 0){
			end = i; 
		}
	}
}

int main(){

	struct stat st = {0};
	
	char directory[30];
	int pid = getpid();
	sprintf(directory, "halleri.rooms.%d", pid);
	//if the directory does not exist, then create it.	
	if(stat(directory, &st) == -1) {
		mkdir(directory, 0755);
	}
	//use as current working directory
	chdir(directory);
		
	srand(time(NULL)); 

	//an array that stores where the pathr has gone
	int path[MAX_ROOMS];
	int next, i, j, randRoom, numCon, countUnused;
	char destination[50];

	getRoom();
	
	//iterate through all rooms	
	for (i=0; i<MAX_ROOMS; i++){
		int *notTaken = malloc(MAX_ROOMS * sizeof(int));
		countUnused = 0;
		
		//assign a random number between 3 and 6 inclusive to be the number of connections
		numCon = rand() % ((MAX_CONNECTIONS+1) - MIN_CONNECTIONS ) + MIN_CONNECTIONS;
		//assign the number of connections to the number of connections for the struct
		rooms[i].numConnections = numCon;
	
		//inner loop 
		for (j=0; j < MAX_ROOMS; j++){
			//j will be the rooms that aren't taken 
       			if (j != i) {
		//		printf("j = %d\t, i = %d\n", j, i);
		//		assign this room to be the rooms not taken in the given array
               			 notTaken[countUnused] = j;
		//		printf("not taken = %d\n", notTaken[countUnused]);
		//		increment count for unused rooms
               			 countUnused++;
           		 }
       		 }
		//iterate through the number of connected rooms
		for (j=0; j<numCon; j++){
			//assign the number of unused rooms to be a random number 
			randRoom = rand() % countUnused;
			//assign the not taken rooms to the array of room connections
			rooms[i].connections[j] = notTaken[randRoom];
			//shuffle the not taken array
			swap(notTaken, randRoom, countUnused - 1);
			//decrement the number of unused rooms
			countUnused--;	
		}
	//free up space
	free(notTaken);
	}

	//create 7 files
	for (i = 0; i < MAX_ROOMS; i++) {

		createFile(rooms[i], i+1);
	}

	printf("\n**********ADVENTURE!**********\n");
	
	int steps = 0;
	while(1){
		printf("CURRENT LOCATION: %s\n", rooms[curr].name);
		printf("POSSIBLE CONNECTIONS: ");
		//iterate through number of room connections
		for (i = 0; i < rooms[curr].numConnections; i++) {
			//if we get to the end
			if(i == (rooms[curr].numConnections - 1)) {
				//print a '.' and end line			
				printf("%s.\n", rooms[rooms[curr].connections[i]].name);
			}
			//other wise add a comma
			else {
				printf("%s, ", rooms[rooms[curr].connections[i]].name);
			}

		}
		//where does the user want to go next? 
		printf("WHERE TO? >");
		fflush(stdout);

		fgets(destination, 50, stdin);
		//breaks strings into tokens
		strtok(destination, "\n");
		printf("\n");
		
		next = find(destination);
		
		for (i=0; i<MAX_ROOMS; i++){
			if(strcmp(rooms[rooms[curr].connections[i]].name, destination)){
				next == -1;
			}
		}
		
		if(next == -1){
			printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
		}
		else{
			curr = next;
			path[steps] = curr;
			steps++;
			if (curr == end){
				printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
				printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
				//print out the path the user took
				for(i=0; i<steps; i++){
					printf("%s\n", rooms[path[i]].name);
					
				}	
				exit(0);			
			}
		}	
	}
}


