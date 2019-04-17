/*Program:    data_uncompress.
  Created by: Gerald Jones
  Purpose:    This is lab 2 for CS360 sp 2015.This program reads from stdin using either getchar or fread to read single
              or multiple chars respectively. It looks for one of these symbols : {n,s,i,d}. These sybmols and the data is 
			  formatted as follows:
			  --if an n is read print a new line
			  --if a d or an i is read that means either a double or an int will be read followed by the number of ints
			    or doubles that will follow.
			  --if an s is read it will follow with the number of strings, and infront of each string will be the number 
			    of chars the string contains.(note fread does note add the NULL, if needed it will have to be added manually
			  The program read will read chars looking for the data above and will store them correctly and print the data 
			  according to the above information as well as doing various error checks*/


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	int i, j, k, num, size,c,go,nmrd;

	FILE * f = stdin;

	int n,s;

	char **buf = NULL;

	char *cp = NULL;			//stands for char pointer

	int * ip = NULL;

	double * dp = NULL;

	int nwln = 1;				//used to check if a new line was printed. Will aide with the formating for the final print

	int strted = 0;

	go = 1;

	int got_data = 0;

	//error check for command line arguments
	if(argc != 1)
	{
		fprintf(stderr,"usage: data_uncompress (then read from std input)\n");
		exit(1);
	}

	/*Now start reading in the data making sure the chars read are either: n,i,d,s and then respoding accordingly*/

	while( c != EOF )
	{

		c = getchar();

		if(c == EOF) exit(0);
		
		/*check for legal input type*/
		if(c == 'n' || (char)c == 's' || c =='i' || c == 'd')
		{

			if( c == 'n')
			{
				printf("\n");
				nwln = 1;				//set the new line flag
				
			}
			else if( c ==  's')
			{


				//get the number of strings and if not print error message and exit
				n = getchar();

				if( n == EOF) 
				{
					fprintf(stderr,"Input error: no size\n"); 
					exit(1);
				}

				//add 1 per instruction from lab write up
				n++;

				//go through the stirngs getting first the number of chars to copy and
				//then copying that number of chars
				for(i = 0 ; i < n ; i++)
				{

					//get the size of the string to print and if not print error message and exit
					s = getchar() ;

					if( s == EOF) 
					{
						fprintf(stderr,"Input error: no string size\n"); 
						exit(1);
					}

					//add 1 per instruction from lab write up
					s++;

					if(cp != NULL) free(cp);

					cp = (char *)malloc( sizeof(char )*s );

					nmrd = fread(cp, sizeof(char), s,f);


					if(nmrd != s) 
					{
						fprintf(stderr,"Input error: not enough chars\n");
						exit(1);
					}


					//check for a new line as the last char read
					//if not add white space to the front ot the string 
					//befor printing the string itself
					if(nwln == 0 ) printf(" ");
					else nwln = 0;
	
					
					for(j = 0; j < nmrd ; j++)
					{
						printf("%c",cp[j]);
					}


				}


			}
			else if( c ==  'i')
			{
				n = getchar() + 1;


				if( n == EOF) 
				{
					fprintf(stderr,"Input error: no interger size\n"); 
					exit(1);
				}


				if(cp != NULL) free(cp);

				cp = (char *) malloc(sizeof(int )* n );

				nmrd = fread( (int *)cp, sizeof(int), n, f);


				//error check for missing information
				if( (int)nmrd != n) 
				{
					fprintf(stderr,"Input error: not enough ints\n");
					exit(0);
				}


				for( j = 0 ; j < n ; j++)
				{
					//the below checks to see if a space needs to be printed
					if(nwln == 0) printf(" ");
					else nwln = 0;

					printf("%d",( (int *) cp) [j]);
				}

			}
			else if( c ==  'd')
			{
				n = getchar() + 1;


				if( n == EOF) 
				{
					fprintf(stderr,"Input error: no double size\n"); 
					exit(1);
				}



				if(cp != NULL) free(cp);

				cp = (char *) malloc(sizeof(double )* n );

				nmrd = fread((double *)cp,sizeof(double),n,f);


				if( nmrd != n) 
				{
					fprintf(stderr,"Input error: not enough doubles\n");
					exit(0);
				}

				for( j = 0 ; j < n ; j++)
				{
					//the below checks to see if a space needs to be printed
					if(nwln == 0 ) printf(" ");
					else nwln = 0;
				
					printf("%.10lg",( (double *)cp) [j]  );
				}

			}

		}
		else
		{
			fprintf(stderr,"Input error: bad type\n");

			exit(0);
		}

	}






	return 0;
}
