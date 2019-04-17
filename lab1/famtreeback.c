#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"
#include "dllist.h"

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


int  is_cycle( person * pp, JRB people)
{
	Dllist tmp;
	JRB bn;

	person * pg;
	
	if( pp->visited == 1) return 0;
	if( pp->visited == 2) return 1;		//there is a cycle

	pp->visited = 2;					//set the current person's v to 2 and if we run into it again there is a cycle
printf("got here 33\n");
	//go through the current persons kids and perform is cycle on them
	dll_traverse(tmp, pp->kids)
	{
		//look in the people tree for this kid
		bn = jrb_find_str(people, jval_s(tmp->val) );
		
		//if the kid is found perform is cycle on it
		if(bn != NULL)
		{
		
			pg = (person *)jval_v(bn->val);

		
			printf("checking %s\n", pg->name);
			if( is_cycle(pg, people ) ) return 1;
		}
		else printf("bn is null\n");
	}

	pp->visited = 1;
	
}

int main(int argc, char **argv)
{
	int i, j, cnt, sz, nm_nms,found;

	person * pp, * pf, * pm, *pg;

	char * cp, *nm;

	char male[5] = "MALE";

	char female[7] = "FEMALE";

	char unknown[8] = "UNKNOWN";




	Dllist tmp, toprint;

	JRB people, bn;

	people = make_jrb();

	IS is = new_inputstruct(NULL);

	while( get_line(is) >= 0 )
	{
		/*Get the line and get the first field to see what it is*/
		if( strcmp( is->fields[0],"PERSON") == 0 )
		{

			//create a person for this line if needed
			//first check if this person already exists
			
			/*Figure out how much space is needed by counting the length of each field beyond the "person" 
			  identifier adding one to each for the null terminator*/
			
			sz = 0;
			cnt = 0;

			for(i = 1; i < is->NF ; i++)
			{
				sz += strlen(is->fields[i]) + 1;
			}

			//printf("the size needed is %d\n",sz);

			/*Using the sz variable and the number of spaces that will be needed( NF -1) allocate space for the name*/
			nm = malloc(sizeof(char )*sz+1);

			//copy the first name into the memory space labeled nm
			strcpy(nm, is->fields[1]);

			
			sz = strlen(is->fields[1]);

			/*Get the name after the person identifier adding spaces after each name*/
			for(i = 2; i < is->NF ; i++)
			{
				nm[sz] = ' ';
				strcpy( nm+sz+1, is->fields[i] );
				sz += strlen( nm+sz );
			}

			/*Look for the person in the tree. If they are not there create a node for them and add them*/
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

				printf("the node for %s was created\n", pp->name);
			}
			else
			{
				
				pp = (person *)jval_v(bn->val);
				printf("%s was already there\n", pp->name);
			}
		
			//the below loop will print the contents of the jrb everytime a person comes up for debugging purposes
			printf("the tree now contains\n");
			jrb_traverse(bn, people)
			{
				printf("----%s\n", jval_s(bn->key) );
			}

		}
		else if( strcmp(is->fields[0],"SEX") == 0)
		{
			/*now get the sex of the current person if it is not already set
			  Otherwise check that the new sex matches the saved on */
			if( pp->sex == "U" )
			{
				pp->sex = strdup( is->fields[1] );
				printf("%s's sex is %c\n",pp->name,(*pp->sex) );
			}
			else 
			{
				if(strcmp( pp->sex, is->fields[1]) != 0) 
				{
					fprintf(stderr,"non mathching sex assingment\n");
					exit(2);
				}

				printf("sex already set\n");
			}

		}
		else if( strcmp(is->fields[0],"FATHER") == 0)
		{
			/*Now get the name of the current persons father and check if it is there,
			  if not add it, then make sure they match if so  ignore, otherwise throw an error*/

			sz = 0;
			cnt = 0;

			//figure out how many chars are in the name then create a string of the name and
			//look for it in the tree
			for(i = 1; i < is->NF ; i++)
			{
				sz += strlen(is->fields[i]) + 1;
			}

			cnt = sz;

			nm  = malloc( sizeof( char )*(sz + 1) );

			strcpy(nm, is->fields[1]);

			sz = strlen(is->fields[1]);

			/*Get the name after the person identifier and create a string with 
			  spaces seperating the names*/
			for(i = 2; i < is->NF ; i++)
			{
				nm[sz] = ' ';
				strcpy( nm+sz+1, is->fields[i] );
				sz += strlen( nm+sz );
			}

			/*Look for the person in the tree if they are not there create a node for them and put
			  them in the tree keyed on there name and a pointer to them as the val*/
			bn = jrb_find_str(people, nm);

			//if the name is not found make a node for it and put it on the tree
			if(bn == NULL)
			{

				pg = (person *)malloc(sizeof(person) );

				pg->name  = malloc( sizeof( char )*(cnt + 1) );
				strcpy(pg->name, nm);
				
				/*make sure the name matches up it there is one*/
				if(pp->father != NULL)
				{
					if(strcmp(pg->name, pp->father->name) != 0)
					{
						
						fprintf(stderr,"There are non matching father assignments\n");
						exit(4);
					}
				}
				pg->mother = NULL;
				pg->father = NULL;
				pg->sex = "M";
				
				pp->father = pg;
				
				pg->kids = new_dllist();

				pg->printed = 0;
				pg->visited = 0;
				dll_append(pg->kids,new_jval_s(pp->name) );
				printf("%s's kids are now...\n",pg->name);
				dll_traverse(tmp, pg->kids) printf("%s \n",jval_s(tmp->val) ); 
				
				jrb_insert_str(people, pg->name, new_jval_v(pg) );
					
				printf("%s was created and added to tree\n", pg->name);


			}
			else		//the person is in the tree
			{
				printf("the name  %s was found\n",nm); 
				/*Make sure the name that was attempted to be added matches the one that is there*/
				pg = (person *)jval_v(bn->val);

				if( pp->father != NULL && strcmp(pg->name, pp->father->name) != 0)
				{
					fprintf(stderr,"There are non matching father assignments\n");
					exit(1);
				}
				else if( pp->father == NULL) pp->father = pg;


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
				else printf("%s is already in %s's kids\n",pp->name, pg->name);

				printf("%s's kids are ...\n",pg->name);
				dll_traverse(tmp, pg->kids) printf("%s \n",jval_s(tmp->val) ); 

				printf("%s was already there in the tree\n", pg->name);

				//check that the sex designation either hasn't been set or matches whats already there
				if(pg->sex == "U")
				{
					printf("the father sex was set\n");
					pg->sex = "M";

				}
				else if( strcmp( pg->sex, "M") != 0)
				{
					fprintf(stderr,"We have a female father\n");
					exit(3);
				}


				printf("%s's father is %s\n",pp->name,pp->father->name);

			}
		}
		else if( strcmp(is->fields[0],"MOTHER") == 0)
		{
			/*Now get the name of the current persons mother and check if it is there,
			  if not add it is so ignore*/


			sz = 0;
			cnt = 0;

			for(i = 1; i < is->NF ; i++)
			{
				sz += strlen(is->fields[i]) + 1;
			}

			cnt = sz;

			nm  = malloc( sizeof( char )*(sz + 1) );

			strcpy(nm, is->fields[1]);

			sz = strlen(is->fields[1]);

			/*Get the name after the person identifier*/
			for(i = 2; i < is->NF ; i++)
			{
				nm[sz] = ' ';
				strcpy( nm+sz+1, is->fields[i] );
				sz += strlen( nm+sz );
			}


			//atempt to find this person in  the tree
			bn = jrb_find_str(people, nm);

			//if not found create kid 
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

				printf("%s's kids are now...\n",pg->name);
				dll_traverse(tmp, pg->kids) printf("%s \n",jval_s(tmp->val) ); 
				
				//insert the mother into the tree
				jrb_insert_str(people, pg->name, new_jval_v(pg) );
					
				printf("%s was created and added to tree\n", pg->name);


			}
			else		//the person is in the tree
			{
				pg = (person *)jval_v(bn->val);

				if( pp->mother == NULL)
				{
					printf("%s's mother was set as %s\n",pp->name,pg->name);
					pp->mother = pg;
				}
				else if( strcmp( pg->name, pp->mother->name) != 0)
				{
					fprintf(stderr,"There are non matching mother assignments\n");
					exit(4);

				}


				if( pg->sex == "U")
				{
					pg->sex = "F";

				}
				else if( strcmp(pg->sex, "F") != 0)
				{

					fprintf(stderr,"We have a male mother\n");
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
				else printf("%s is already in %s's kids\n",pp->name, pg->name);
				printf("%s's mother is %s\n",pp->name,pp->mother->name);
				printf("%s's kids are now...\n",pg->name);
				dll_traverse(tmp, pg->kids) printf("%s \n",jval_s(tmp->val) ); 
				printf("%s was already in the tree \n", pg->name);
			}
		}
		else if( strcmp(is->fields[0],"FATHER_OF") == 0)
		{
			/*Get the name of the current person's child and if they don't already
			  exist create them and set their father to current person */				

			printf("%s's sex is %s\n", pp->name, pp->sex);

			if(pp->sex != "U" && pp->sex == "F")
			{
				fprintf(stderr,"We have a female father of\n");
				exit(9);
			}

			sz = 0;
			cnt = 0;

			if( pp->sex == "U")
			{
				pp->sex = "M";
			}

				
			//create key for name by first calculating the size of memory needed
			//and then using the fields to recreate the name with spaces inbetween each name
			for(i = 1; i < is->NF ; i++)
			{
				sz += strlen(is->fields[i]) + 1;
			}

			cnt = sz;

			nm  = malloc( sizeof( char )*(sz + 1) );

			strcpy(nm, is->fields[1]);

			sz = strlen(is->fields[1]);

			/*Get the name after the person identifier*/
			for(i = 2; i < is->NF ; i++)
			{
				nm[sz] = ' ';
				strcpy( nm+sz+1, is->fields[i] );
				sz += strlen( nm+sz );
			}

			//check for the name in the tree
			//if not there create it and set up the appropriate links
			//if it is there make sure the name, and sex is what it should be 
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

				printf("%s's kids are now...\n",pp->name);
				dll_traverse(tmp, pp->kids) printf("%s \n",jval_s(tmp->val) ); 

				jrb_insert_str(people, pg->name, new_jval_v(pg) );
					
				printf("%s was created and added to tree\n", pg->name);

			}
			else  //the person was in the tree
			{
				pg = (person *)jval_v(bn->val);
				printf("%s was already there\n", pg->name);

				if( pg->father == NULL)
				{
					pg->father = pp;
					printf("the father of %s is %s\n", pg->name, pg->father->name);
				}

				else if( strcmp( pg->father->name,pp->name) != 0)
				{
					fprintf(stderr,"we have 2 fathers of %s\n",pg->name);
					exit(9);
				}

				if( (pp->sex) == "U") pp->sex = "M";
				else if(pp->sex == "F")
				{

					fprintf(stderr,"we have 2 fathers of %s\n",pg->name);
					exit(9);
				}

				found = 0;

				//look for the kid in pp's kids
				dll_traverse(tmp, pp->kids)
				{
					if( strcmp(jval_s(tmp->val), pg->name) == 0) found =1;


				}

				if(found) printf("%s is already in %s's kids\n",pg->name, pp->name);
				else
				{
					pg->father = pp;
					printf("the child is %s and their father is %s\n", pg->name, pg->father->name);

					printf("%s's kids are...\n", pg->father->name);
					/*now add this child to the current person's kids*/
					dll_append(pp->kids,new_jval_s(pg->name) );

					dll_traverse(tmp, pp->kids) printf("%s \n",jval_s(tmp->val) );
					}
				
			}

		}
		else if( strcmp(is->fields[0],"MOTHER_OF") == 0)
		{
			/*Get the name of the current person's child and if they don't already
			  exist create them and set their father to current person */				

			if(pp->sex != "U" && pp->sex != "F")
			{
				fprintf(stderr,"We have a male mother of\n");
				exit(9);
			}

			sz = 0;
			cnt = 0;

			for(i = 1; i < is->NF ; i++)
			{
				sz += strlen(is->fields[i]) + 1;
			}

			cnt = sz;

			nm  = malloc( sizeof( char )*(sz + 1) );

			strcpy(nm, is->fields[1]);

			sz = strlen(is->fields[1]);

			/*Get the name after the person identifier*/
			for(i = 2; i < is->NF ; i++)
			{
				nm[sz] = ' ';
				strcpy( nm+sz+1, is->fields[i] );
				sz += strlen( nm+sz );
			}

			bn = jrb_find_str(people, nm);

			if(bn == NULL)
			{

				if(pp->sex == "M")
				{
					fprintf(stderr,"we have a male mother\n");
					exit(8);
				}

				pp->sex = "F";

				pg = (person *)malloc(sizeof(person) );

				pg->name  = malloc( sizeof( char )*(cnt + 1) );
				pg->sex = malloc(sizeof(char) );
				pg->sex = "U";
				pg->father = NULL;
				pg->mother = pp;
				strcpy(pg->name, nm);

				pp->printed = 0;
				pg->visited = 0;
				pg->kids = new_dllist();
				dll_append(pp->kids,new_jval_s(pg->name) );

				printf("%s's kids are now...\n",pp->name);
				dll_traverse(tmp, pp->kids) printf("%s \n",jval_s(tmp->val) ); 

				jrb_insert_str(people, pg->name, new_jval_v(pg) );
					
				printf("%s was added to tree\n", pg->name);

			}
			else   
			{
				//the person was in the tree so if the kids mother is not set it. Then
				//,otherwise make sure the sex of the mother is right, if not throw an error
				//then check if the name of the mother is correct, if not throw the 
				//error, Then see if you have to add the kid
				pg = (person *)jval_v(bn->val);
				printf("%s was already there\n", pg->name);

				if(pg->mother == NULL)
				{
					pg->mother = pp;

					printf("the mother of %s is %s\n", pg->name, pg->mother->name);
				}
				else if(strcmp(pg->mother->name, pp->name) !=0)
				{

					fprintf(stderr,"we have 2 mothers of %s\n",pg->name);
					exit(9);
				}
				
				if( pp->sex  == "U") pp->sex = "F";
				else if( strcmp(pp->sex, "F") != 0)
				{
					printf("in side the else\n");
					fprintf(stderr,"we a male mothers of %s\n",pg->name);
					exit(9);
				}

				printf("the childs name is %s and their mother is  %s\n",pg->name, pg->mother->name);
				
				//look for the child in pp'skids
				found = 0;

				dll_traverse(tmp, pp->kids) 
				{
					if(strcmp(jval_s(tmp->val), pg->name) == 0)
					{
						found = 1;
					}
				
				}
				
				if(!found) dll_append(pp->kids,new_jval_s(pg->name) );
				else printf("the kid is already there\n");


				printf("%s's kids are...\n", pg->mother->name);
				/*now add this child to the current person's kids*/


				dll_traverse(tmp, pp->kids) printf("%s \n",jval_s(tmp->val) ); 

			}
		}
		else if( strcmp( is->fields[0] , "SHOW_T") == 0)
		{
			
			jrb_traverse(bn, people)
			{
				
//				printf("%s %c\n", jval_s(bn->key), (*(  (person *)jval_v(bn->val))->sex)  );
			
				pg = (person *)jval_v(bn->val);

				if(pg->sex == "U") printf("%s %s\n", pg->name, unknown);
				else if(pg->sex == "F") printf("%s %s\n", pg->name, female);
				else if(pg->sex == "M") printf("%s %s\n", pg->name, male);

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

 int Is_c = 0;

	//now do a dfs to check for a cycle
	jrb_traverse(bn, people)
	{
		pg = (person* )jval_v(bn->val);
		
		Is_c = is_cycle( pg, people);

		if( Is_c == 1) 
		{
			fprintf(stderr,"there is a cycle\n");

			exit(6);
		}
		
	}


	//now print out the people with the required formatting
	//first look through tree finding anyone with no kids and adding them to toprint
	jrb_traverse(bn,people)
	{
		pg = (person* )jval_v(bn->val);
		
		if( dll_empty( pg->kids) )
		{
			printf("%s has no kids\n");
			dll_append(toprint, new_jval_s(pg->name));

		}

	}


	if(dll_empty(toprint) ) dll_append(toprint,new_jval_s(pp->name) );

	while( !dll_empty(toprint) )
	{
		

		dll_delete_node( dll_first(toprint));

		//check bn's printed field
		
		pg = (person*)jval_v(bn->val);

		//checking pg's printed
		if(pg->printed = 0)
		{
			if(pg->father == NULL && pg->mother == NULL || pg->father->printed == 1 && pg->mother->printed == 1)
			{
				printf("%s\n", pg->name);

				if(pg->sex == "M")
				{
					printf("SEX:%s\n", male);
				}
				else if(pg->sex == "F")
				{
					printf("SEX:%s\n", female);

				}
				else if(pg->sex == "U")
				{

					printf("SEX:%s\n", unknown);
				}

				if(pg->father != NULL) printf("FATHER: %s\n", pg->father->name);
				else printf("FATHER: %s\n",unknown);

				if(pg->mother != NULL) printf("MOTHER: %s\n", pg->mother->name);
				else printf("MOTHER: %s\n",unknown);


				printf("CHILDREN: ");
				//now print the kids 
				dll_traverse(tmp, pg->kids) 
				{
					nm = jval_s(tmp->val);
					dll_append(toprint, new_jval_s(nm) );
					printf("%s add to to print", jval_s(tmp->val) );

				}
				printf("\n");
			}


		}

	}

	return 0;
}

