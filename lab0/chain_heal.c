/*Program: chain_heal.c
  Created by: Gerald Jones 
  Purpose:    This is lab 0 for cs360. It is a lab designed to help transistion from c++ to c programming. It uses examples of 
			  allocating memory with malloc, casting variables to various types, and setting up stucts in c. A set of constraints
			  are given as command line arguments in the form:
			  "./a.out intial_cast_range jump_range jump_limit initial_healing__pp power_reduction
			  A set of players is given on stdin and one will always be the healer Urgosa. The program enumerates all the possible 
			  healing orders and finds the one that results in the most pp healed. This is then printed to stdout as well as the 
			  total healing possible for this order*/
			  

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


/*This struct represents a player that could possibly be healed*/
typedef struct plr
{
	int x;
	int y;
	int crnt_pp;				/*Current pp for this player*/
	int max_pp;					/*The max pp that this player can have*/
	char  name[100];			
	struct plr * prev;			/*this pointer is used to point to another player in some sequence*/
	int adj_size;				
	int visited;
	struct plr **adj;			/*This points to the players adjacent list*/
	int healing;				/*This is used to track the amount of pp healed for this player*/
}player;

/*This struct will be used to hold global information*/
typedef struct information
{
	int c_j;			//cap on jump number
	double c_pp;		//the current amount of pp healed per current jump	
	int amnt_heal;		//the current amount of healing done in this dfs 
	int best_heal;		//the best amount of healing done on a DFS
	double c_p_r;		//current power reduction
	int * healing;		//points to the healing values 
	player **best_path;	//an array of pointers to players in the order of the best path
	int best_path_length;
}info;

/*this will perform a Depth first search on all nodes that are children of the current player*/
void DFS( player * pp, int hop, info * Ip, int ttl_heal, player * from)
{
	int i, j, rslt, dfs_heal, old_pp, cnt;
	
	player * ptr;

	double prv_pp, num;

	cnt = 0;

	/*if the node has already been visited in this dfs just return*/
	if( (*pp).visited != -1) return;
	
	/*Check to make sure that the hop limit is not exceeded if so just return*/
	if( hop > Ip->c_j)
	{
		return;
	}

	(*pp).visited = 1;

	(*pp).prev = from;					/*set the previous field to the node that came before*/
	

	/*Save the state of the player and heal amounts before the DFS*/
	old_pp = (*pp).crnt_pp;			

	prv_pp = Ip->c_pp;					

	dfs_heal = Ip->amnt_heal;			
	
	
	
	/*get how much healing would occur if the max is not exceeded*/
	rslt = Ip->c_pp + pp->crnt_pp;	


	/*the below will make sure the players max pp is not exceeded, and calculate
	  the appropriate amount of healing done one the current node and thus far.
	  It also sets the currents nodes new pp*/
	if( rslt > (*pp).max_pp)
	{
		Ip->amnt_heal += (*pp).max_pp - (*pp).crnt_pp;	
		pp->healing  = (*pp).max_pp - (*pp).crnt_pp;
		ttl_heal += (*pp).max_pp - (*pp).crnt_pp;	
		(*pp).crnt_pp = (*pp).max_pp;			

	}
	else
	{
		Ip->amnt_heal += rint( Ip->c_pp );					
		ttl_heal += rint( Ip->c_pp );					
		(*pp).crnt_pp = rslt;						

		pp->healing  = rint(Ip->c_pp);
		
	}

	
	/*now check this new total heal amount against your best heal and if it beats it replace the old with the new
	  and store the path that got you here*/
	if( ttl_heal > Ip->best_heal)
	{

		Ip->best_heal = ttl_heal;


		Ip->best_path_length = hop;


		ptr = pp;

		for( i = Ip->best_path_length - 1 ; i >= 0 ; i--)
		{
			(Ip->best_path[i]) = ptr;

			(Ip->healing[i]) = ptr->healing;

			ptr = ptr->prev;
		}

	}
	
	//adjust the next jumps pp healed using the power reduction
	Ip->c_pp =  Ip->c_pp * (1 - (Ip->c_p_r)) ;


	for(i = 0; i < (*pp).adj_size ; i++)
	{
		ptr = (*pp).adj[i];		

		DFS( ptr, hop+1, Ip, ttl_heal ,pp);

	}



	//set everything back to the way it was

	(*pp).visited = -1;

	Ip->c_pp = prv_pp;

	Ip->amnt_heal = dfs_heal;

	(*pp).crnt_pp = old_pp;

	return;
}


int main( int argc, char** argv)
{

	char hldr[100];

	double dist;

	int player_cnt, x, y, c_pp, mx_pp, i, j;

    player * pp;				

	player * prv = NULL;				/*will store a pointer to the previous player*/

	player * UP = NULL;					/*Will be used to point to the player Urgosa*/

	player_cnt = 0;
	//process command line arguments and store in global information struct
	
	info *Ip = malloc(sizeof(info) );
	
	Ip->c_j = atoi(argv[3]);
	Ip->c_pp = atoi(argv[4]);
	sscanf(argv[5],"%lf",&Ip->c_p_r);
	Ip->amnt_heal = 0;
	Ip->best_heal = 0;


	Ip->best_path = (player **) malloc( sizeof(player *) * atoi(argv[3]) );

	Ip->healing = (int *) malloc( sizeof( int * ) * atoi(argv[3]) );
	





	//Now take in lines from std input and create nodes for each line/player
	
	while( scanf("%d %d %d %d %s", &x, &y, &c_pp, &mx_pp, hldr) > 0 )
	{
		
		pp = (player *)malloc( sizeof(player) );

		pp->x = x;
		(*pp).y = y;
		(*pp).crnt_pp = c_pp;
		(*pp).max_pp = mx_pp;
		(*pp).visited = -1;
		(*pp).healing = 0;
		(*pp).adj_size = 0;
		strcpy( (*pp).name, hldr);
		(*pp).prev = prv;
		
		/*check for Urgosa
		  if found save a pointer to it
		  Then set the current player as the previous player for linking purposes*/

		if( strcmp(hldr, "Urgosa_the_Healing_Shaman") == 0) UP = pp; 
		
		prv = pp;

		player_cnt++;

	}



	player *nodes[player_cnt];				/*will hold all of the nodes in the graph*/
	player *edges[player_cnt];				/*will be used as a temporary player pointer contianer*/

	//now make the graph by going through the nodes using there previous fields
	//adding them to the graph array
	for( i=0 ; i < player_cnt ; i++)
	{
	
		nodes[i] = prv;
		prv = (*prv).prev;
	}



	//go through all nodes and set the adjacent lists based on jump range
	for( i = 0; i < player_cnt; i++)
	{

		//check the needed size of adjacent array for current node
		//by going through and counting how many are possible based on the distance from current node
		for(j = 0; j < player_cnt; j++)
		{
			//make sure not to count the node it self
			if(j != i)
			{
				x = pow( (double) ( (*nodes[i]).x - (*nodes[j]).x) ,2);
				y = pow( (double) ( (*nodes[i]).y - (*nodes[j]).y) ,2);
				dist = ( sqrt( x + y) );


				/*check for those players that are within jump range if so add to edges temporarily and increment cnt*/
				
				if(dist <= atoi(argv[2])) 
				{
					edges[ nodes[i]->adj_size] = nodes[j];	

					nodes[i]->adj_size++;
				}
			}


		}


		nodes[i]->adj = (player **)malloc(sizeof(player *)* (nodes[i]->adj_size) );

		for(j = 0; j < nodes[i]->adj_size  ; j++)
		{
			nodes[i]->adj[j] = edges[j];
		}

	}

	/*check the distance of the each node from Urgosa and if it is 
	  within the initial cast range perform DFS on it to enumerate all 
	  possible paths of healing and find the one with the most pp healed*/


	for(i = 0; i < player_cnt ; i++)
	{
		
		x = pow( ( (double) ( (*UP).x - (*nodes[i]).x ) ), 2 );
		y = pow( ( (double) ( (*UP).y - (*nodes[i]).y ) ), 2 );

		dist = ( sqrt(x + y) );

		if(dist <= atoi(argv[1])) 
		{
			
			DFS( nodes[i] , 1 , Ip, 0 ,NULL); 
		
		}
	}

	/*Now print the best path with the name of the player and the amount they were healed. 
	  Then print out the total healing done*/
	for(i = 0; i < Ip->best_path_length ;i++)
	{
		printf("%s %d\n", (Ip->best_path[i])->name, Ip->healing[i] );
	}

	printf("Total_Healing %d\n", Ip->best_heal); 
	
	return 0;
}
