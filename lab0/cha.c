#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct plr
{
	int x;
	int y;
	int crnt_pp;
	int max_pp;
	char * name;
	struct plr * prev;
}player;


int main( int argc, char** argv)
{

	//process command line arguments
	int i_rng = (int)(*argv[1]) - '0';			//initial range
	int j_rng = (int)(*argv[2]) - '0';			//jump range
	int nm_j  = (int)(*argv[3]) - '0';			//number of jumps
	int i_pwr = (int)(*argv[4]) - '0';			//initial power
	double  pwr_r ;								//power range
	
	sscanf(argv[5],"%lf",&pwr_r);
	
	printf("the i range-%d, jmp range-%d, number jmps-%d, i pwr-%d, pwr rd-%.2lf\n",i_rng, j_rng, nm_j, i_pwr, pwr_r);	

	char hldr[100];

	

	int player_cnt, x, y, c_pp, mx_pp;

	int input = 1;


    player * pp;	

	player * prv = NULL;

	player * pa[50];

	player_cnt = 0;

	//now take in lines from std input and create nodes for each line/player
	while(input != EOF)
	{
		//allocate memory for a player and get the pointer to it	
		pp = (player *)malloc( sizeof(player) );

		printf("got here\n");




		input = scanf("%d",&(*pp).x);
		
		if(input != EOF)
		{
			scanf("%d",&(*pp).y);
			scanf("%d",&(*pp).crnt_pp);
			scanf("%d",&(*pp).max_pp);
			scanf("%s",(*pp).name);
		
			(*pp).prev = prv;
			prv = pp;

		//	pa[player_cnt] = pp;

			player_cnt++;
		}
		

	}

	printf("\nthe player count is %d\n", player_cnt );
	printf("\nthe size of and int * is %d\n", sizeof(int *)  );

	int i = 0;

	player *nodes[player_cnt];

	//now make the graph by going through the nodes using there previous fields
	//adding them to the graph array
	for( i ; i < player_cnt ; i++)
	{
		printf("the %d player is %s at (%d,%d), with %d mx pp and %d c pp\n",i, (*pa[i]).name,(*pa[i]).x,(*pa[i]).y,(*pa[i]).max_pp,(*pa[i]).crnt_pp);
	
		nodes[i] = prv;
		prv = (*prv).prev;
	
	}

	printf("\n\n");

	for( i = 0; i < player_cnt; i++)
	{
		printf("the %d element in the node array is %s \n", i , (*nodes[i]).name);
	}

	return 0;
}
