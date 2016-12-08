/* CS261- Assignment 2 - calc.c/
/* Name: Eric Hall
* Date: Monday, June 29, 2015
 * Solution description: Makes use of the functions from dynamicArray.c to implement a calculator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dynamicArray.h"

/* param: s the string
 * param: num a pointer to double
 * returns: true (1) if s is a number else 0 or false.
 * postcondition: if it is a number, num will hold
 * the value of the number
 */
int isNumber(char *s, double *num) {
	char *end;
	double returnNum;

	if(strcmp(s, "0") == 0) {
		*num = 0;
		return 1;
	}
	else {
		returnNum = strtod(s, &end);
		/* If there's anything in end, it's bad */
		if((returnNum != 0.0) && (strcmp(end, "") == 0)) {
			*num = returnNum;
			return 1;
		}
	}
	return 0; // if we arrived here, it was not a number
}

/*	param: stack the stack being manipulated
 *	pre:	the stack contains at least two elements
 *	post: 	the top two elements are popped and
 *	their sum is pushed back onto the stack.
 */
void add(struct DynArr *stack){

	double a, b, c;

	if(sizeDynArr(stack) < 2){
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);


	b = topDynArr(stack);
	popDynArr(stack);

	c = a + b;
	pushDynArr(stack, c);

}

/* 	param: stack the stack being manipualted
 *  	pre: 	the stack contains at least two elements
 *  	post:	the top two elements are popped and
 *  	their difference is pushed back onto the stack.
 */
void subtract(struct DynArr *stack){

	double a, b, c;

	if(sizeDynArr(stack) < 2) {
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);
	b = topDynArr(stack);
	popDynArr(stack);

	c = b - a;

	pushDynArr(stack, c);
}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least two elements
 *	post:	the top two elements are popped and
 *	their product is pushed back onto the stack
 */
void multiply(struct DynArr *stack){

	double a, b, c;

	if(sizeDynArr(stack) < 2){
		printf("Not enough elements.\n");
	}
	a = topDynArr(stack);
	popDynArr(stack);
	b = topDynArr(stack);
	popDynArr(stack);

	c = a * b;

	pushDynArr(stack, c);
}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least two elements
 *	post:	the top two elements are popped and
 *	their quotient is pushed back onto the stack
 */
void divide(struct DynArr *stack){

	double a, b, c;

	if(sizeDynArr(stack) < 2) {
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);

	b = topDynArr(stack);
	popDynArr(stack);

	c = b/a;

	pushDynArr(stack, c);
}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least two elements
 *	post:	the top two elements are popped and
 *	their power is pushed onto the stack
 */
void power(struct DynArr *stack){

	double a, b, c;

	if(sizeDynArr(stack) < 2){
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);

	b = topDynArr(stack);
	popDynArr(stack);
	c = pow(b, a);

	pushDynArr(stack, c);

}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least one element
 *	post:	the top element is popped and
 *	its square is pushed onto the stack.
 */
void square(struct DynArr *stack){

	double a, b;

	if(sizeDynArr(stack) < 1){
		printf("Not enough elements");
	}

	a = topDynArr(stack);
	popDynArr(stack);

	b = a * a;

	pushDynArr(stack, b);

}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least one element
 *	post:	the top element is popped
 *	and its cube is pushed onto the stack.
 */
void cube(struct DynArr *stack){

	double a, b;

	if(sizeDynArr(stack) < 1){
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);

	b = a * a * a;

	pushDynArr(stack, b);
}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least one element
 *	post:	the top element is popped
 *	and its absolute value is pushed onto the stack
 */

void absoluteValue(struct DynArr *stack){

	double a, b;

	if(sizeDynArr(stack) < 1){
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);

	b = fabs(a);

	pushDynArr(stack, b);

}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least one element
 *	post:	the top element is popped and
 *	its square root is pushed onto the stack.
 */

void squareRoot(struct DynArr *stack) {

	double a, b;

	if(sizeDynArr(stack) < 1) {
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);

	b = sqrt(a);

	pushDynArr(stack, b);

}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least one element
 *	post:	the top element is popped and
 *	its exponential is pushed back onto the stack.
 */

void exponential(struct DynArr *stack){

	double a, b;

	if(sizeDynArr(stack) < 1){
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);

	b = exp(a);

	pushDynArr(stack, b);

}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least one element
 *	post:	the top element is popped and
 *	its natural logarithm is pushed back onto the stack
 */

void naturalLog(struct DynArr *stack){

	double a, b;

	if(sizeDynArr(stack) < 1){
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);
	b = log(a);

	pushDynArr(stack, b);

}

/*	param:	stack the stack being manipulated
 *	pre:	the stack contains at least one element
 *	post:	the top element is popped and
 *	its logarithm is pushed back onto the stack.
 */

void logBase10(struct DynArr *stack){

	double a, b;

	if(sizeDynArr(stack) < 1){
		printf("Not enough elements.\n");
	}

	a = topDynArr(stack);
	popDynArr(stack);
	b = log10(a);

	pushDynArr(stack, b);

}

double calculate (int numInputTokens, char **inputString) {
	int i;
	double result = 0.0;
	char *s;
	struct DynArr *stack;

	double *p = malloc(sizeof(double));

	//set up the stack
	stack = createDynArr(20);

	//start at 1 to skip the name of the calculator calc
	for(i = 1; i < numInputTokens; i++) {
		s = inputString[i];

		//Hint: General algorithm:
		//(1) Check if the string s is in the list of operators.
		//	(1a) If it is, perform corresponding operations.
		//	(1b) Otherwise, check if s is a number.
		//	(1b - I) If s is not a number, produce an error.
		//	(1b - II) If s is a number, push it onto the stack

		if(strcmp(s, "+") == 0)
			add(stack);
		else if (strcmp(s, "-") == 0)
			subtract(stack);
		else if (strcmp(s, "/") == 0)
			divide(stack);
		else if (strcmp(s, "x") == 0)
			multiply(stack);
		else if (strcmp(s, "^") == 0)
			power(stack);
		else if (strcmp(s, "^2") == 0)
			square(stack);
		else if (strcmp(s, "^3") == 0)
			cube(stack);
		else if (strcmp(s, "abs") == 0)
			absoluteValue(stack);
		else if (strcmp(s, "sqrt") == 0)
			squareRoot(stack);
		else if (strcmp(s, "exp") == 0)
			exponential(stack);
		else if (strcmp(s, "ln") == 0)
			naturalLog(stack);
		else if (strcmp(s, "log") == 0)
			logBase10(stack);
		else {
			//FIXME: You need to develop the code here (when s is not an
			//operator)
			//Remember to deal with values ("pi" and "e')

			if(strcmp(s, "pi") == 0) {
				pushDynArr(stack, 3.14159);
			}
			else if(strcmp(s, "e") == 0) {
				pushDynArr(stack, 2.71828);
			}

			else{

				if(isNumber(s, p) == 1) {
					pushDynArr(stack, *p);
				}
				else{
					printf("Invalid.\n");
					printf("The offender was %s\n", s);
				}
			}
		}

	}

	/* FIXME: You will write this part of the function (2 steps below)
	* (1) Check if everything looks OK and produce an error if needed.
	* (2) Store the final value in result and print it out
	*/


	if(sizeDynArr(stack) == 1) {
		result = topDynArr(stack);

	printf("Result %.2f\n", result);
	}
	else{
		printf("Your equation is not balanced.\n");
	}
	return result;


}

int main(int argc, char** argv) {

	// assume each argument is contained in the argv array
	// argc-1 determines the number of operands + operators
	if(argc == 1)
		return 0;

	calculate(argc, argv);
	return 0;
}
