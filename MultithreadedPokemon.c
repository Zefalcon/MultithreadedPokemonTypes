/** Multithreaded Pokemon Type Chart Searcher
 * @author Evelyn Moss
 * @author Michael Huang
 */

#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 20
#define MAX_TYPES 4

int found[5]; //Marks whether the specific type is found.
int indices[5]; //Marks the indices the specific types are at in the chart.
int multiplier = 1;
const char *type[18]; /* Type List */
const int chart[18][18]; /* Type Chart */

int main(){

	*type = { "Normal", "Fighting", "Flying", "Poison", "Ground", "Rock", "Bug", "Ghost", "Steel",
		"Fire", "Water", "Grass", "Electric", "Psychic", "Ice", "Dragon", "Dark", "Fairy" };

	chart[0] = { 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	chart[1] = { 1, 1, 2, 1, 1,.5,.5, 1, 1, 1, 1, 1, 1, 2, 1, 1,.5, 2 };
	chart[2] = { 1,.5, 1, 1, 0, 2,.5, 1, 1, 1, 1,.5, 2, 1, 2, 1, 1, 1 };
	chart[3] = { 1,.5, 1,.5, 2, 1,.5, 1, 1, 1, 1,.5, 1, 2, 1, 1, 1,.5 };
	chart[4] = { 1, 1, 1,.5, 1,.5, 1, 1, 1, 1, 2, 2, 0, 1, 2, 1, 1, 1 };
	chart[5] = { .5, 2,.5,.5, 2, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1 };
	chart[6] = { 1,.5, 2, 1,.5, 2, 1, 1, 1, 2, 1,.5, 1, 1, 1, 1, 1, 1 };
	chart[7] = { 0, 0, 1,.5, 1, 1,.5, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1 };
	chart[8] = { .5, 2,.5, 0, 2,.5,.5, 1,.5, 2, 1,.5, 1,.5,.5,.5, 1,.5 };
	chart[9] = { 1, 1, 1, 1, 2, 2,.5, 1,.5,.5, 2,.5, 1, 1,.5, 1, 1,.5 };
	chart[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1,.5,.5, 2, 2, 1,.5, 1, 1, 1 };
	chart[11] = { 1, 1, 2, 2,.5, 1, 2, 1, 1, 2,.5,.5,.5, 1, 2, 1, 1, 1 };
	chart[12] = { 1,.5, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1,.5, 1, 1, 1, 1, 1 };
	chart[13] = { 1,.5, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1,.5, 1, 1, 2, 1 };
	chart[14] = { 1, 2, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 1, 1,.5, 1, 1, 1 };
	chart[15] = { 1, 1, 1, 1, 1, 1, 1, 1, 1,.5,.5,.5,.5, 1, 2, 2, 1, 2 };
	chart[16] = { 1, 2, 1, 1, 1, 1, 2,.5, 1, 1, 1, 1, 1, 1, 1, 1,.5, 2 };
	chart[17] = { 1,.5, 1, 2, 1, 1,.5, 1, 2, 1, 1, 1, 1, 1, 1, 0,.5, 1 };

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
	if(num > MAX_TYPES){
		printf("Maximum number of types exceeded.  Please try again");
		return 0;
	}
	char *defend[num][MAX_LINE/2 + 1]; //stores defending types
	//Get defending types
	int j = 0;
	while(j < num){
		printf("Enter defending type %d: ", j); //May need /n somewhere?
		gets(line);
		defend[j][0] = strtok(line, " ");
	}
	int k;
	for(k = 0; k < num + 1; k++){
		found[k] = 0;
	}
	

	int i;
	omp_set_num_threads(omp_get_max_threads()); //Ensures omp uses the max number of threads for search.

	#pragma omp parallel for private(tid)
	for (i = 0; i < num + 1; i++){
		//Split available threads into num+1 groups
		tid = omp_get_thread_num();
		int threads_per_group = omp_get_num_threads() / (num + 1);
		int search_section_size = 18 / threads_per_group;
		int group_num; //Group number, for use in thread divisions
		int first_num; //First thread number of the group, for use in thread divisions
		int relative_num; //Thread number relative to group, for use in thread divisions

		//Group 0 searches for attack[0]
		if (tid < threads_per_group){
			group_num = 0;
			first_num = group_num * threads_per_group;
			relative_num = tid - first_num;

			//Search divided between threads by relative thread number, with the first thread getting the first part, the second the second part, etc.
			int t;
			for(t = relative_num * search_section_size; t < (relative_num + 1) * search_section_size && !found[group_num]; t++){
				if(type[t] == attack[0]){
					#pragma omp critical {
						indices[group_num] = t;
						found[group_num] = 1;
					}
				}
			}
		}

		//Groups 1->num search for defend[0->num-1]
		else {
			else if (tid < threads_per_group * 2){
				group_num = 1;
			}
			else if (tid < threads_per_group * 3){
				group_num = 2;
	
			}
			else if (tid < threads_per_group * 4){
				group_num = 3;
			}
			else{
				group_num = 4;
			}
		
			first_num = group_num * threads_per_group;
			relative_num = tid - first_num;		

			//Search divided between threads by relative thread number, with the first thread getting the first part, the second the second part, etc.
			int t;
			for(t = relative_num * search_section_size; t < (relative_num + 1) * search_section_size && !found[group_num]; t++){
				if(type[t] == defend[group_num - 1]){
					#pragma omp critical {
						indices[group_num] = t;
						found[group_num] = 1;
					}
				}
			}
		}
	}

	//Return total multiplier
	int defend;
	int attack = indices[0];  //indices[0] is attack type, all else defense.
	for(defend = 0; defend < num; r++){
		//Defending type is first index, Attacking is second.
		multiplier = multiplier * chart[defend][attack];
	}

	printf("Total damage multiplier: %d", multiplier);
}
	
	
	/*
	
	//           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17

	

	//           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17


																0) Normal; takes 2x from Fighting (1); takes 0x from Ghost (7); else takes 1x
																1) Fighting; takes 2x from (2, 13, 17); takes 1/2x from (5, 6, 16); else takes 1x
																2) Flying; takes 2x from (5, 12, 14); takes 1/2 x from (1, 6, 11); takes 0x from (4); else takes 1x
																3) Poison; takes 2x from (4, 13); takes 1/2x from (1, 3, 6, 11, 17); else takes 1x
																4) Ground; takes 2x from(10, 11, 14); takes 1/2x from (3, 5); takes 0x from Electric (12); else takes 1x
																5) Rock; takes 2x from (1, 4, 8, 10, 11); takes 1/2x from Normal, Flying, Poison, Fire (0, 1, 3, 9); else takes 1x
																6) Bug; takes 2x from (2, 5, 9); takes 1/2x from Fighting, Ground, Grass (1, 4, 11); else takes 1x
																7) Ghost; takes 2x from (7, 16); takes 1/2x from (3, 6); takes 0x from Normal, Fighting (0, 1); else takes 1x
																8) Steel; takes 2x from Fighting, Ground, Fire (1, 4, 9); takes 1/2x from (0, 2, 5, 6, 8, 11, 13, 14, 15, 17); takes 0x from (3); else takes 1x
																9) Fire; takes 2x from (4, 5, 10); takes 1/2x from (6, 8, 9, 11, 14, 17); else takes 1x
																10) Water; takes 2x from (11, 12); takes 1/2x from (9, 10, 14); else takes 1x
																11) Grass; takes 2x from (2, 3, 6, 9, 14); takes 1/2x from (4, 10, 11, 12); else 1x
																12) Electric; takes 2x from (4); takes 1/2x from (2, 12); else 1x
	13) Psychic; takes 2x from (6, 7, 16); takes 1/2x from (1, 13); else 1x
	14) Ice; takes 2x from (1, 5, 8, 9); takes 1/2x from (14); else 1x
	15) Dragon; takes 2x from (14, 15, 17); takes 1/2x from (9, 10, 11, 13); else 1x
	16) Dark; takes 2x from (1, 6, 17); takes 1/2x from (7, 16); takes 0x from (13); else 1x
	17) Fairy; takes 2x from (3, 8); takes 1/2x from (1, 6, 16); takes 0x from (15); else 1x
	
	if (Defend = X) {
	    if (attack = Supereffective)
	        multiplier = multiplier * 2;
		else if (attack = NotEffective)
			multiplier = multiplier/2;
		else if (attack = immune)
			multiplier = multiplier * 0;
		else
			multiplier = multiplier * 1;
	}
	*/