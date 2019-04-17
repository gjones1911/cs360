/*Program:    data_uncompress.
  Created by: Gerald Jones
  Purpose:    This is lab 2 for CS360 sp 2015.This program reads from stdin using either getchar or */


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	int i, j, k, num, size,c,go,nmrd;

	//set the file to be read as stdin
	FILE * f = stdin;

	//unsigned char /*buf[10],*/ n, s;
	int n,s;
	char **buf = NULL;

	char *cp = NULL;			//stands for char pointer

	int * ip = NULL;

	double * dp = NULL;

	int nwln = 1;

	int strted = 0;

	go = 1;

	int got_data = 0;

	//error check for command line arguments
	if(argc != 1)
	{
		fprintf(stderr,"usage: data_uncompress (then read from std input)\n");
		exit(1);
	}

	/*Now start reading in the data making sure the chars read are: n,i,d,s*/

	while( c != EOF )
	{
		/*read in a char and check what to do next*/

		c = getchar();

		if(c == EOF) exit(0);
		
		
		

		//ignore the new line

		//printf("getchar got  %c\n",c);


		/*check for legal input type*/
		if(c == 'n' || (char)c == 's' || c =='i' || c == 'd')
		{

			if( c == 'n')
			{
				printf("\n");
				nwln = 1;
				
			}
			else if( c ==  's')
			{
				//			printf("got a s!! \n");


				//get the number and if not print error message and exit
				n = getchar();

				//		printf("the n is 0x%lx or in value %c\n", (unsigned long)n, n);

				if( n == EOF) 
				{
					fprintf(stderr,"Input error: no size\n"); 
					exit(1);
				}

				n++;


				//		printf("the number of strings to copy is %d  ",n);

				//go through the stirngs getting first the number of chars to copy and
				//then copying that number of chars using fread
				for(i = 0 ; i < n ; i++)
				{

					//get the size of the string to print and if not print error message and exit
					s = getchar() ;

					if( s == EOF) 
					{
						fprintf(stderr,"Input error: no string size\n"); 
						exit(1);
					}
					s++;
					//						printf("the size of the string is %d ",s);

					if(cp != NULL) free(cp);

					cp = (char *)malloc( sizeof(char )*s );			//allcocate the correct amount of memory

					nmrd = fread(cp, sizeof(char), s,f);

					//						printf("the number of chars read is %d\n",nmrd);

					if(nmrd != s) 
					{
						fprintf(stderr,"Input error: not enough chars\n");
						exit(1);
					}

					//						printf("we got ..\n");

					if(nwln == 0 ) printf(" ");
					else nwln = 0;
					for(j = 0; j < nmrd ; j++)
					{
						printf("%c",cp[j]);
					}

					strted = 1;

				}


			}
			else if( c ==  'i')
			{
				//					printf("got a i\n");
				n = getchar() + 1;

				//					printf(" n is now %d\n",n);

				if( n == EOF) 
				{
					fprintf(stderr,"Input error: no interger size\n"); 
					exit(1);
				}

				//	num = (int)(n - '0' + 1);

				//					printf("the number of ints to get is %d\n",n);

				if(cp != NULL) free(cp);

				cp = (char *) malloc(sizeof(int )* n );

				nmrd = fread( (int *)cp, sizeof(int), n, f);

				//					printf("the number of reads is %d\n",nmrd);

				if( (int)nmrd != n) 
				{
					fprintf(stderr,"Input error: not enough ints\n");
					exit(0);
				}


				for( j = 0 ; j < n ; j++)
				{



					//						printf("We got ...\n");

					    if(nwln == 0) printf(" ");
						else nwln = 0;
						printf("%d",( (int *) cp) [j]);
				}
				strted = 1;

			}
			else if( c ==  'd')
			{
				//					printf("got a d\n");
				n = getchar() + 1;

				//					printf(" n is now %d\n",n);

				if( n == EOF) 
				{
					fprintf(stderr,"Input error: no double size\n"); 
					exit(1);
				}

				//num = (int)(n - '0' + 1);

				//					printf("the number of doubles to get is %d\n",n);

				if(cp != NULL) free(cp);

				cp = (char *) malloc(sizeof(double )* n );

				nmrd = fread((double *)cp,sizeof(double),n,f);


				//					printf("the number of reads is %d\n",nmrd);

				if( nmrd != n) 
				{
					fprintf(stderr,"Input error: not enough doubles\n");
					exit(0);
				}

				for( j = 0 ; j < n ; j++)
				{


					//						printf("We got ...\n");

						if(nwln == 0 ) printf(" ");
						else nwln = 0;
						printf("%.10lg",( (double *)cp) [j]  );
				}
					strted = 1;

			}


		}
		else
		{
			fprintf(stderr,"Input error: bad type\n");

			exit(0);
		}

		strted = 1;
	}






	return 0;
}
