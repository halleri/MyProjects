
/* CS261- Assignment 4 - compare.c/
/* Name: Eric Hall
* Date: Monday, July 13, 2015
 * Solution description: Compares and prints data in a struct */


#include <stdio.h>
#include <assert.h>
#include "bst.h"
#include "structs.h"

/*----------------------------------------------------------------------------
 very similar to the compareTo method in java or the strcmp function in c. it
 returns an integer to tell you if the left value is greater then, less then, or
 equal to the right value. you are comparing the number variable, letter is not
 used in the comparison.

 if left < right return -1
 if left > right return 1
 if left = right return 0
 */

 /*Define this function, type casting the value of void * to the desired type.
  The current definition of TYPE in bst.h is void*, which means that left and
  right are void pointers. To compare left and right, you should first cast
  left and right to the corresponding pointer type (struct data *), and then
  compare the values pointed by the casted pointers.

  DO NOT compare the addresses pointed by left and right, i.e. "if (left < right)",
  which is really wrong.
 */
int compare(TYPE left, TYPE right)
{
    /*FIXME: write this*/
	assert (left != 0);
	assert (right != 0);
	struct data *l = left;
	struct data *r = right;

	if (l->number > r->number) {
		return 1;
	}
	else if (l->number < r->number) {
		return -1;
	}

	return 0;

}

/*Define this function, type casting the value of void * to the desired type*/
void print_type(TYPE curval)
{
    /*FIXME: write this*/
	assert (curval != 0);
	struct data* cur = (struct data *) curval;

	printf("Value: %d", cur->number);

}


