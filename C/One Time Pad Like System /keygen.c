#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Please enter a key length");
		exit(0);
	}

	srand(time(NULL));

	int randChar;
	int keyLength = atoi(argv[1]);	// get key length and convert to int

	int i;
	//loop through key length
	for (i = 0; i < keyLength; i++) {
		//generate a random character
		randChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[rand()%27];
		//if the random char is 26 + ascii 'A' (which is 65)
		if (randChar == 26 + 'A') {
			//assign a space to random character
			randChar = ' ';
		}
		//otherwise print out that character
		fprintf(stdout, "%c", randChar);
	}
	//print new line
	fprintf(stdout, "\n");

	return 0;
}

