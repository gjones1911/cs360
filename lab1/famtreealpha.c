#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"
#include "dllist.h"

typedef struct per 
{
	char ** name;
	char * sex;
	struct per *father;
	struct per *mother; 
	Dllist kids;;
	int nm_cnt;				//saves the number of names 

}person;


int main(int argc, char **argv)
{
	int i, j, cnt, sz, nm_nms;

	person * pp, * pf, * pm, *pg;

	char * cp;

	Dllist tmp;

	JRB people;

	IS is = new_inputstruct(NULL);

	while( get_line(is) >= 0 )
	{
		/*Get the line and get the first field to see what it is*/
		if( strcmp( is->fields[0],"PERSON") == 0 )
		{

			//create a person for this line if needed
			//first check if this person already exists
			
			nm_nms = NF -1;				//figure out how many names this person has




			if(jrb_find_str(people, 
			pp = (person *)malloc(sizeof( person ) );
				
			pp->kids = new_dllist();

			/*Figure out how much space is needed by counting the fields length*/
			
			sz = 0;
			cnt = 0;

			for(i = 1; i < is->NF ; i++)
			{
				sz += strlen(is->fields[i]);
			}

			printf("the size needed is %d and the number of names is %d\n",sz, is->NF-1);

			/*Using the sz variable and the number of spaces that will be needed( NF -1) allocate space for the name*/
			pp->name  = malloc( sizeof( char * )*(is->NF-1) );
printf("the nf is %d \n", is->NF);

			pp->nm_cnt = 0;

			/*Get the name after the person identifier*/
			for(i = 1; i < is->NF ; i++)
			{
				pp->name[i - 1] = strdup(is->fields[i] );
				pp->nm_cnt++;;
			}


			printf("the name count is %d and the name is ",pp->nm_cnt);

			for(i = 0; i < pp->nm_cnt ; i++)
			{
				printf("%s",(pp->name[i]) );
				if(i != pp->nm_cnt - 1) printf(" ");	

			}

			printf("\n");
		}
		else if( strcmp(is->fields[0],"SEX") == 0)
		{
			/*now get the sex of the current person*/
			pp->sex = strdup( is->fields[1] );

			printf("the %s's sex is %c\n",pp->name[0],(*pp->sex) );

		}
		else if( strcmp(is->fields[0],"FATHER") == 0)
		{
			/*Now get the name of the current persons father and check if it is there,
			  if not add it is so ignore*/

			printf("the prob is here 1\n");
			pf = (person *)malloc(sizeof(person) );

			pf->sex = malloc( sizeof(char *) );
			(*pf->sex) = 'M';

			printf("the prob is here 2\n");

			pf->name = malloc( sizeof(char *) * is->NF-1);

			for(i = 1; i < is->NF ; i++)
			{
				pf->name[i-1] = strdup(is->fields[i] );
			}

			pf->nm_cnt = i - 1;

			pp->father = pf;

			printf("%s's father is %s who has %d names\n",pp->name[0],pp->father->name[0], pp->father->nm_cnt);

		}
		else if( strcmp(is->fields[0],"MOTHER") == 0)
		{
			/*Now get the name of the current persons mother and check if it is there,
			  if not add it is so ignore*/

			pm = (person *)malloc(sizeof(person) );

			pm->sex = malloc( sizeof(char *) );
			(*pm->sex) = 'F';

			pm->name = malloc( sizeof(char *) * is->NF-1);

			for(i = 1; i < is->NF ; i++)
			{
				pm->name[i-1] = strdup(is->fields[i] );
			}

			pm->nm_cnt = i - 1;

			pp->mother = pm;

			printf("%s's mother is %s who has %d names\n",pp->name[0],pp->mother->name[0], pp->mother->nm_cnt);

		}
		else if( strcmp(is->fields[0],"FATHER_OF") == 0)
		{
			/*Get the name of the current person's child and if they don't already
			  exist create them and set their father to current person */				
			
			pg = (person *)malloc(sizeof(person) );

			pg->father = pp;

			pg->name = malloc( sizeof(char *) * is->NF-1);
			
			for( i = 0; i < is->NF ; i++)
			{
				pg->name[i-1] = strdup(is->fields[i] );

			}

			pg->nm_cnt = i -1; 

			printf("the childs name is %s and they have %d names\n",pg->name[0], pg->nm_cnt);


			/*now add this child to the current person's kids*/
			dll_append(pp->kids,new_jval_s(pg->name[0]) );

			dll_traverse(tmp, pp->kids) printf("%s \n",jval_s(tmp->val) ); 
			
				
			

	
		}
		else if( strcmp(is->fields[0],"MOTHER_OF") == 0)
		{
			/*Get the name of the current person's child and if they don't already
			  exist create them and set their father to current person */				
			
			pg = (person *)malloc(sizeof(person) );

			pg->mother = pp;

			pg->name = malloc( sizeof(char *) * is->NF-1);
			
			for( i = 0; i < is->NF ; i++)
			{
				pg->name[i-1] = strdup(is->fields[i] );

			}

			pg->nm_cnt = i -1; 

			printf("the childs name is %s and they have %d names\n",pg->name[0], pg->nm_cnt);


			/*now add this child to the current person's kids*/
			dll_append(pp->kids,new_jval_s(pg->name[0]) );

			dll_traverse(tmp, pp->kids) printf("%s \n",jval_s(tmp->val) ); 

		}

	}
	return 0;
}

