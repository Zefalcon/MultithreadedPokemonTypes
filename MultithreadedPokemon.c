/** Multithreaded Pokemon Type Chart Searcher
 * @author Evelyn Moss
 * @author Michael Huang
 */

#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 20

int found = 0;  //Need one of these for each group somehow...
int multiplier = 1;
const char *type[18]; /* Type List */
const int chart[18][18]; /* Type Chart */

int main(){
	int tid;
	char line[20];

	//Get attack type
	char *attack[MAX_LINE/2 + 1];
	printf("Enter attack type: ");
	gets(line);
	attack[0] = strtok(line, " ");

	//Get number of defending types
	int num;
	printf("Enter number of defending types: ");
	gets(line);
	num = strtok(line, " ");
	if(num > 4)
		printf("Number of types may only go up to 4.  Please try again.");
	else {
		//Get defending types
		int j = 0;
		char *defend[num][MAX_LINE/2 + 1]; //stores defending types
		while(j < num){
			printf("Enter defending type %d: ", j); //May need /n somewhere?
			gets(line);
			defend[j][0] = strtok(line, " ");
		}
	}

	//Split available threads into num+1 groups
	
	//Group 1 searches for attack[0]

	//Groups 2->num search for defend[0->num]

	//Return total multiplier
	printf("Total damage multiplier: %d", multiplier);
}