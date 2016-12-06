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
	//TODO: LOTS of copy-pasta.  Relegate to separate section.
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
					//TODO: Critical
					found[group_num] = 1;
				}
			}
		}

		//Groups 1->num search for defend[0->num-1]
		//TODO: See if these divisions can be made less static
		else if (tid < threads_per_group * 2){
			group_num = 1;
			first_num = group_num * threads_per_group;
			relative_num = tid - first_num;

			//Search divided between threads by relative thread number, with the first thread getting the first part, the second the second part, etc.
			int t;
			for(t = relative_num * search_section_size; t < (relative_num + 1) * search_section_size && !found[group_num]; t++){
				if(type[t] == defend[group_num - 1]){
					//TODO: Critical
					found[group_num] = 1;
				}
			}
		}
		else if (tid < threads_per_group * 3){
			group_num = 2;
			first_num = group_num * threads_per_group;
			relative_num = tid - first_num;	

			//Search divided between threads by relative thread number, with the first thread getting the first part, the second the second part, etc.
			int t;
			for(t = relative_num * search_section_size; t < (relative_num + 1) * search_section_size && !found[group_num]; t++){
				if(type[t] == defend[group_num - 1]){
					//TODO: Critical
					found[group_num] = 1;
				}
			}		
		}
		else if (tid < threads_per_group * 4){
			group_num = 3;
			first_num = group_num * threads_per_group;
			relative_num = tid - first_num;		

			//Search divided between threads by relative thread number, with the first thread getting the first part, the second the second part, etc.
			int t;
			for(t = relative_num * search_section_size; t < (relative_num + 1) * search_section_size && !found[group_num]; t++){
				if(type[t] == defend[group_num - 1]){
					//TODO: Critical
					found[group_num] = 1;
				}
			}
		}
		else{
			group_num = 4;
			first_num = group_num * threads_per_group;
			relative_num = tid - first_num;		

			//Search divided between threads by relative thread number, with the first thread getting the first part, the second the second part, etc.
			int t;
			for(t = relative_num * search_section_size; t < (relative_num + 1) * search_section_size && !found[group_num]; t++){
				if(type[t] == defend[group_num - 1]){
					//TODO: Critical
					found[group_num] = 1;
				}
			}
		}

	//Return total multiplier
	//TODO: After each type is found, find multiplier and multiply.
	printf("Total damage multiplier: %d", multiplier);
}