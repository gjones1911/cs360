#include <cstdio.h>
#include <cstdlib.h>
#include "fields.h"
//#include "jrb.h"
//#include "dllist.h"
//#include "jvals.h"

int main( int argc , char ** argv)
{
	int i , j, cnt, tmp;

	char * cp
	char ** cpp;


	IS code, input;

	/*process command line arguments and name them something I can keep track of*/

	if( argc != 2)
	{
		printf("--usage ./a.out code_def input_file");
		exit(0);
	}

   code  = new_iputstruct(argv[1]);

	if( code == NULL )
	{
		perror(argv[1]);
		exit(1);
	}

    input = new_inputstruct(argv2[2]);

	if( input == NULL )
	{
		perror();
		exit(1);
	}

    getline(code);

	printf(" the size of the word is %d\n", input->NF);




	return 0;

}
