#include <stdio.h>
#include <stdlib.h>
#include "fields.h"
#include <string.h>
//#include "jrb.h"
//#include "dllist.h"
//#include "jvals.h"

int main( int argc , char ** argv)
{
	int i , j, cnt, tmp, c;

	char * cp;
	char ** cpp;
	
	int  hld[100];

	FILE * FPC;
	FILE * FPE;


	FPC = fopen(argv[1],"r");


//	IS input;

	/*process command line arguments and name them something I can keep track of*/

	i = 0;
	cnt = 0;

	printf("the value of null is %d\n",(int)'\0');
	
   do{

		c = fgetc( FPC );
//		cp = code->text1 + i;
    	if(c != EOF && c !='\0'  )
		{
			printf("the char is %c\n",(char) c);
		}
		else if(c =='\0') 
		{
			printf("found a space\n");
			hld[cnt] = i;
			 cnt++;
		}
		i++;
	}while(c != EOF);

	if( argc != 2)
	{
		printf("--usage ./hd code_def input_file");
		exit(0);
	}




/*
    input = new_inputstruct(argv[2]);

	if( input == NULL )
	{
		perror(argv[2]);
		exit(1);
	}
*/
	i = 0;

	printf("the value of new line is %d\n", '\n');
cnt = 0;

	while( c != EOF )
	{
		c = fgetc(FPC); 
		cp = *argv + i;
	
		if(c == -1) printf( "c didnt get it\n") ; exit(0);

//		if(c != EOF )
//		{   
			
			

				//printf(" the size of the line is %d\n", code->NF);

				printf("the char is  %c\n",(char) c );
			

				j = strlen(cp);

				printf("the number of char here is %d\n", j);
				for(i = 0 ; i < j; i++)
				{

				}

				i++;
		//}
		//
		//
	}
		/*
		   do 
		   /{
	//	c  = getchar( );
	//	printf("the char just got is %c\n", (char)c);
	} while( c != EOF );
	*/
	
	return 0;

}
