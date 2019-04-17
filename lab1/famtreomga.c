#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"
#include "dllist.h"

/*A struct representing a person*/
typedef struct per 
{
	char * name;
	char * sex;
	struct per *father;
	struct per *mother; 
	Dllist kids;
	int visited;
	int printed;

}person;

/*This is a DFS that will return a 1 if if finds a cyle(a person who is their own decendant)*/
int  is_cycle( person * pp, JRB people)
{
	Dllist tmp;
	JRB bn;

	person * pg, *father, *mother;
	
	if( pp->visited == 1) return 0;
	if( pp->visited == 2) return 1;		//there is a cycle

	pp->visited = 2;					//set the current person's v to 2 and if we run into it again there is a cycle
	
	//go through the current persons kids and perform is_cycle on them
	dll_traverse(tmp, pp->kids)
	{
		bn = jrb_find_str(people, jval_s(tmp->val) );
		
		if(bn != NULL )
		{
		
			pg = (person *)jval_v(bn->val);
		
			if( is_cycle(pg, people ) == 1 )
			{
				return 1;
			}
		}
	}

	pp->visited = 1;

	return 0;
	
}


/*The main program will take in information on stdin and process and store it as needed into a 
  RB tree. The program will then check for cycles, and if none are found it will print the information in the 
  required format making sure to print parents befor children*/

int main(int argc, char **argv)
{
	int i, j, cnt, sz, nm_nms,found;

	person * pp, * father, * mother, *pg;

	char * cp, *nm;

	char male[5] = "Male";

	char female[7] = "Female";

	char unknown[8] = "Unknown";

	nm = NULL;


	Dllist tmp, toprint;

	JRB people, bn;

	people = make_jrb();

	IS is = new_inputstruct(NULL);

	while( get_line(is) >= 0 )
	{
		//Ignore the empty lines
		if( is->NF != 0)
		{

			if( strcmp( is->fields[0],"PERSON") == 0 )
			{

				//Create a key made of the names of the person seperated spaces and then 
				//check if this person already exists within the tree

				sz = 0;
				cnt = 0;

				for(i = 1; i < is->NF ; i++)
				{
					sz += strlen(is->fields[i]) + 1;
				}

				if(nm != NULL) free(nm);
				nm = malloc(sizeof(char )*sz+1);

				//copy the first name into the memory space labeled nm(a.k.a name)
				strcpy(nm, is->fields[1]);


				sz = strlen(is->fields[1]);

				for(i = 2; i < is->NF ; i++)
				{
					nm[sz] = ' ';
					strcpy( nm+sz+1, is->fields[i] );
					sz += strlen( nm+sz );
				}

				/*Look for the person in the tree. If they are not there create a node for it and add them to tree
				  adjusting the appropriate fields of the new node. If it is in the tree */
				bn = jrb_find_str(people,nm);


				if( bn == NULL)
				{
					pp = (person *)malloc(sizeof( person ) );

					pp->name  = malloc( sizeof( char )*(sz + 1) );
					//pp->sex = malloc( sizeof(char)*8 );
					pp->sex = "U" ;
					pp->father = NULL;
					pp->mother = NULL;
					strcpy(pp->name, nm);
					pp->kids = new_dllist();
					pp->printed = 0;
					pp->visited = 0;

					jrb_insert_str(people, pp->name, new_jval_v(pp) );

				}
				else
				{
					/*If it is in the tree already get a pointer to this person */

					pp = (person *)jval_v(bn->val);
				}

			}
			else if( strcmp(is->fields[0],"SEX") == 0)
			{
				/*Now set the sex of the current person if it is not already set
				  Otherwise check that the new sex matches the saved one */
				if( pp->sex == "U" )
				{
					if( strcmp( "M", is->fields[1] ) == 0)
					{
						pp->sex = "M";
					}
					else
					{
						pp->sex = "F";
					}
				}
				else 
				{
					if(strcmp( pp->sex, is->fields[1]) != 0) 
					{
						fprintf(stderr,"Bad input - sex mismatch on line %d\n",is->line);
						exit(2);
					}

				}

			}
				/*For the FATHER/MOTHER lines of input get the name of the suppossed father/mother and see 
				 * if it is the tree. If not add it, then make sure that if the current persons father 
				 * is set it matches what was attempted to be added as the father.if so  ignore, 
				 * otherwise throw an error*/
			else if( strcmp(is->fields[0],"FATHER") == 0)
			{

				sz = 0;
				cnt = 0;

				for(i = 1; i < is->NF ; i++)
				{
					sz += strlen(is->fields[i]) + 1;
				}

				cnt = sz;

				if(nm != NULL) free(nm);
				nm  = malloc( sizeof( char )*(sz + 1) );

				strcpy(nm, is->fields[1]);

				sz = strlen(is->fields[1]);

				for(i = 2; i < is->NF ; i++)
				{
					nm[sz] = ' ';
					strcpy( nm+sz+1, is->fields[i] );
					sz += strlen( nm+sz );
				}

				if( pp->father != NULL && strcmp(pp->father->name, nm) != 0 )
				{
					fprintf(stderr,"Bad input -- child with two fathers on line %d\n", is->line);
					exit(9);
				}


				bn = jrb_find_str(people, nm);

				if(bn == NULL)
				{

					pg = (person *)malloc(sizeof(person) );

					pg->name  = malloc( sizeof( char )*(cnt + 1) );
					strcpy(pg->name, nm);

					pg->mother = NULL;
					pg->father = NULL;
					pg->sex = "M";

					pp->father = pg;

					pg->kids = new_dllist();

					pg->printed = 0;
					pg->visited = 0;
					dll_append(pg->kids,new_jval_s(pp->name) );

					jrb_insert_str(people, pg->name, new_jval_v(pg) );


				}
				else		//the person is in the tree
				{
					pg = (person *)jval_v(bn->val);

					pp->father = pg;


					found = 0;

					//look for the kid to be added in the dlist already
					dll_traverse(tmp, pg->kids) 
					{
						if( strcmp( jval_s(tmp->val), pp->name) == 0 )
						{
							found = 1;
						}

					}
					//add the current person as one of the father kids if they are not already there
					if(!found) dll_append(pg->kids,new_jval_s(pp->name) );

					//check that the sex designation either hasn't been set or matches whats already there
					if(pg->sex == "U")
					{
						//printf("the father sex was set\n");
						pg->sex = "M";

					}
					else if( strcmp( pg->sex, "M") != 0)
					{
						fprintf(stderr,"Bad input - sex mismatch on line %d\n",is->line);
						exit(3);
					}


					//		printf("%s's father is %s\n",pp->name,pp->father->name);

				}
			}
			else if( strcmp(is->fields[0],"MOTHER") == 0)
			{


				sz = 0;
				cnt = 0;

				for(i = 1; i < is->NF ; i++)
				{
					sz += strlen(is->fields[i]) + 1;
				}

				cnt = sz;

				if(nm != NULL) free(nm);
				nm  = malloc( sizeof( char )*(sz + 1) );

				strcpy(nm, is->fields[1]);

				sz = strlen(is->fields[1]);

				for(i = 2; i < is->NF ; i++)
				{
					nm[sz] = ' ';
					strcpy( nm+sz+1, is->fields[i] );
					sz += strlen( nm+sz );
				}

				if( pp->mother != NULL && strcmp(pp->mother->name, nm) != 0 )
				{
					fprintf(stderr,"Bad input -- child with two mothers on line %d\n", is->line);
					exit(9);
				}

				bn = jrb_find_str(people, nm);

				if(bn == NULL)
				{

					pg = (person *)malloc(sizeof(person) );

					pg->name  = malloc( sizeof( char )*(cnt + 1) );
					strcpy(pg->name, nm);



					pg->father = NULL;
					pg->mother = NULL;

					pg->sex = "F";
					pp->mother = pg;

					pp->printed = 0;
					pg->visited = 0;
					pg->kids = new_dllist();

					//add the current person as one of the mother kids
					dll_append(pg->kids,new_jval_s(pp->name) );

					jrb_insert_str(people, pg->name, new_jval_v(pg) );



				}
				else		//the person is in the tree
				{
					pg = (person *)jval_v(bn->val);

					pp->mother = pg;


					if( pg->sex == "U")
					{
						pg->sex = "F";

					}
					else if( strcmp(pg->sex, "F") != 0)
					{

						fprintf(stderr,"Bad input - sex mismatch on line %d\n",is->line);
						exit(4);
					}

					found = 0;

					//look for the kid to be added in the dlist already
					dll_traverse(tmp, pg->kids) 
					{
						if( strcmp( jval_s(tmp->val), pp->name) == 0 )
						{
							found = 1;
						}

					}
					//add the current person as one of the father kids
					if(!found) dll_append(pg->kids,new_jval_s(pp->name) );
				}
			}
			else if( strcmp(is->fields[0],"FATHER_OF") == 0)
			{

				if(pp->sex != "U" && pp->sex == "F")
				{
					fprintf(stderr,"Bad input - sex mismatch on line %d\n", is->line);
					exit(9);
				}

				sz = 0;
				cnt = 0;

				if( pp->sex == "U")
				{
					pp->sex = "M";
				}


				for(i = 1; i < is->NF ; i++)
				{
					sz += strlen(is->fields[i]) + 1;
				}

				cnt = sz;

				if(nm != NULL) free(nm);
				nm  = malloc( sizeof( char )*(sz + 1) );

				strcpy(nm, is->fields[1]);

				sz = strlen(is->fields[1]);

				for(i = 2; i < is->NF ; i++)
				{
					nm[sz] = ' ';
					strcpy( nm+sz+1, is->fields[i] );
					sz += strlen( nm+sz );
				}


				bn = jrb_find_str(people, nm);


				if(bn == NULL)
				{

					pg = (person *)malloc(sizeof(person) );

					pg->name  = malloc( sizeof( char )*(cnt + 1) );
					pg->sex = "U";
					pg->father = pp;
					pg->mother = NULL;
					strcpy(pg->name, nm);
					pg->kids = new_dllist();

					pp->printed = 0;
					pg->visited = 0;
					pp->sex = "M";

					//need tpo look for the kid in pp's dllist
					dll_append(pp->kids,new_jval_s(pg->name) );


					jrb_insert_str(people, pg->name, new_jval_v(pg) );


				}
				else  //the person was in the tree
				{
					pg = (person *)jval_v(bn->val);

					if( pg->father == NULL)
					{
						pg->father = pp;
					}

					else if( strcmp( pg->father->name,pp->name) != 0)
					{
						fprintf(stderr,"Bad input -- two fathers on line %d\n",is->line);
						exit(9);
					}

					pp->sex = "M";

					found = 0;

					//look for the kid in pp's kids
					dll_traverse(tmp, pp->kids)
					{
						if( strcmp(jval_s(tmp->val), pg->name) == 0) found =1;


					}

					//	if(found) printf("%s is already in %s's kids\n",pg->name, pp->name);
					if(!found)
					{
						pg->father = pp;

						dll_append(pp->kids,new_jval_s(pg->name) );

					}

				}

			}
			else if( strcmp(is->fields[0],"MOTHER_OF") == 0)
			{
				/*Get the name of the current person's child and if they don't already
				  exist create them and set their father to current person */				

				if(pp->sex != "U" && pp->sex != "F")
				{
					fprintf(stderr,"Bad input - sex mismatch on line %d\n", is->line);
					exit(9);
				}

				sz = 0;
				cnt = 0;

				for(i = 1; i < is->NF ; i++)
				{
					sz += strlen(is->fields[i]) + 1;
				}

				cnt = sz;

				if(nm != NULL) free(nm);
				nm  = malloc( sizeof( char )*(sz + 1) );

				strcpy(nm, is->fields[1]);

				sz = strlen(is->fields[1]);

				for(i = 2; i < is->NF ; i++)
				{
					nm[sz] = ' ';
					strcpy( nm+sz+1, is->fields[i] );
					sz += strlen( nm+sz );
				}


				bn = jrb_find_str(people, nm);

				if(bn == NULL)
				{

					pp->sex = "F";

					pg = (person *)malloc(sizeof(person) );

					pg->name  = malloc( sizeof( char )*(cnt + 1) );
					pg->sex = "U";
					pg->father = NULL;
					pg->mother = pp;
					strcpy(pg->name, nm);

					pp->printed = 0;
					pg->visited = 0;
					pg->kids = new_dllist();
					dll_append(pp->kids,new_jval_s(pg->name) );


					jrb_insert_str(people, pg->name, new_jval_v(pg) );


				}
				else   
				{
					//the person was in the tree so if the kids mother is not set it. Then
					//,otherwise make sure the sex of the mother is right, if not throw an error
					//then check if the name of the mother is correct, if not throw the 
					//error, Then see if you have to add the kid
					pg = (person *)jval_v(bn->val);

					if(pg->mother == NULL)
					{
						pg->mother = pp;

					}
					else if(strcmp(pg->mother->name, pp->name) !=0)
					{

						fprintf(stderr,"Bad input -- two mothers on line %d\n",is->line);
						exit(9);
					}

					pp->sex = "F";

					found = 0;

					dll_traverse(tmp, pp->kids) 
					{
						if(strcmp(jval_s(tmp->val), pg->name) == 0)
						{
							found = 1;
						}

					}

					if(!found) dll_append(pp->kids,new_jval_s(pg->name) );

				}
			}
			else if( strcmp( is->fields[0] , "SHOW_T") == 0)
			{

				printf("---------SHOW_T---------\n");

				jrb_traverse(bn, people)
				{


					pg = (person *)jval_v(bn->val);

					if(pg->sex == "U") printf("----%s %s\n", pg->name, unknown);
					else if(pg->sex == "F") printf("----%s %s\n", pg->name, female);
					else if(pg->sex == "M") printf("----%s %s\n", pg->name, male);

					if( !dll_empty( pg->kids) )printf("%s's kids are...\n", pg->name);
					else printf("%s's kids : NONE",pg->name);

					dll_traverse(tmp, pg->kids) 
					{
						printf("%-2s ",jval_s(tmp->val) ); 

					}

					printf("\n");
				}
			}
			else if( strcmp(is->fields[0], "CP") == 0)
			{
				printf("-------------the current person is %s\n", pp->name);
			}
		}
	}

	int Is_c = 0;

	//now do a dfs to check for a cycle
	jrb_traverse(bn, people)
	{
		pg = (person* )jval_v(bn->val);

		Is_c = is_cycle( pg, people);


		if(  is_cycle( pg, people) )
		{
			fprintf(stderr,"Bad input -- cycle in specification\n");

			exit(6);
		}

	}

	toprint = new_dllist();

	//now print out the people with the required formatting
	//first look through tree finding anyone with no kids and adding them to toprint
	jrb_traverse(bn,people)
	{
		pg = (person* )jval_v(bn->val);

		dll_append(toprint, new_jval_s(pg->name));


	}


	int doit = 0;

	//perform a BFS to make sure you print parents before childre
	while( !dll_empty(toprint) )
	{


		tmp = dll_first(toprint);


		bn = jrb_find_str(people, jval_s(tmp->val));

		dll_delete_node( dll_first(toprint));


		pg = (person*)jval_v(bn->val);

		father = pg->father;
		mother = pg->mother;

		if(father == NULL && mother == NULL) doit = 1;
		else if(mother !=  NULL && mother->printed == 1 && father != NULL && father->printed == 1) doit = 1;
		else if(mother ==  NULL && father != NULL && father->printed == 1) doit = 1;
		else if(mother !=  NULL && mother->printed == 1 && father == NULL ) doit = 1;
		else  doit = 0; 


		if(pg->printed == 0)
		{
			if(doit)
			{
				printf("%s\n", pg->name);

				pg->printed = 1;

				if(pg->sex == "M")
				{
					printf("  Sex: %s\n", male);
				}
				else if(pg->sex == "F")
				{
					printf("  Sex: %s\n", female);

				}
				else 
				{

					printf("  Sex: %s\n", unknown);
				}

				if(pg->father != NULL) printf("  Father: %s\n", pg->father->name);
				else printf("  Father: %s\n",unknown);

				if(pg->mother != NULL) printf("  Mother: %s\n", pg->mother->name);
				else printf("  Mother: %s\n",unknown);


				printf("  Children: ");
				if( dll_empty(pg->kids) ) printf("None\n");

				else
				{
					printf("\n");
					dll_traverse(tmp, pg->kids) 
					{
						nm = jval_s(tmp->val);
						dll_append(toprint, new_jval_s(nm) );
						printf("    %s\n",jval_s(tmp->val) );

					}
				}
				printf("\n");
			}
			else
			{
				dll_append(toprint, new_jval_s(pg->name));
			}
		}
	}

	return 0;
}

