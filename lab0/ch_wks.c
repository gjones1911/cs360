#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


/*This struct represents a player that could possibly be healed*/
typedef struct plr
{
	int x;
	int y;
	int crnt_pp;
	int max_pp;
	char  name[100];
	struct plr * prev;
	int adj_size;
	int visited;
	struct plr **adj;
	int healing;
}player;

/*This struct will be used to hold global information*/
typedef struct information
{
	int i_rng;		//initial range
	int j_rng;		//jump range
	int c_j;		//cap on jump number
	int Init_pp;   //the intial power of pp  healead
	double c_pp;		//the current amount of pp healed per current jump	
	int amnt_heal;  //the current amount of healing done in this dfs 
	int best_heal;	//the best amount of healing done on a DFS
	double c_p_r;		//current power reduction
	int * healing;		//points to the healing values 
	player **best_path;
	int best_path_length;
}info;

/*this will hopefully perform a Depth first search on all nodes that are children of the current player*/
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
//		printf("the hop is %d and the cap on jumps is %d\n",hop, Ip->c_j);
//		printf("The total amount healed is %d\n",Ip->amnt_heal );
		
		return;
	}

//	printf("--------------------------------------------------%s, hop %d\n",(*pp).name, hop);
	(*pp).visited = 1;

	(*pp).prev = from;					/*set the previous field to the node that came before*/
	
	old_pp = (*pp).crnt_pp;				/*save a pointer to the original pp of this child*/

	prv_pp = Ip->c_pp;					/*save the current pp healed per jump*/

	rslt = Ip->c_pp + pp->crnt_pp;		/*get how much healing would occur if the max is not exceeded*/

	dfs_heal = Ip->amnt_heal;			/*save the amount of healing done up to this dfs*/

//	printf("the amount healed before this hop is %d \n", Ip->amnt_heal);

//	printf("the current pp given before this hop is %d\n",Ip->c_pp);



	/*the below will make sure the players max pp is not exceeded*/
	if( rslt > (*pp).max_pp)
	{
		Ip->amnt_heal += (*pp).max_pp - (*pp).crnt_pp;		//the amount healed will be the difference of the current and max pp 
		pp->healing  = (*pp).max_pp - (*pp).crnt_pp;
		ttl_heal += (*pp).max_pp - (*pp).crnt_pp;		//the amount healed will be the difference of the current and max pp 
		(*pp).crnt_pp = (*pp).max_pp;							/*set the childs current pp to the max*/
		

//		printf("%s's heal is now %d\n", pp->name, pp->healing);	
//		printf("The possible amount healed on this jump is %d\n", pp->max_pp - pp->crnt_pp);
//		printf("the new total amount healed is %d \n", Ip->amnt_heal);
//		printf("the New total amount healed is %d \n", ttl_heal);

	}
	else
	{
		Ip->amnt_heal += rint( Ip->c_pp );					/*the amount healed is the pp that is added + what was added before*/
		ttl_heal += rint( Ip->c_pp );								/*the amount healed is the pp that is added + what was added before*/
		(*pp).crnt_pp = rslt;									/*set the current childs pp to the sum of its old and the crnt heal*/

		
		pp->healing  = rint(Ip->c_pp);
		
//		printf("%s's heal is now %d\n", pp->name, pp->healing);	
//		printf("The amount healed on this jump is %d\n", Ip->c_pp);
//		printf("the new total amount healed is %d \n", Ip->amnt_heal);
//		printf("the New total amount healed is %d \n", ttl_heal);

	}

	
	/*now check this new total heal amount against your best heal and if it beats it replace the old with the new*/
	if( ttl_heal > Ip->best_heal)
	{
//		printf("the best heal is now %d\n", ttl_heal );

		Ip->best_heal = ttl_heal;

		//Ip->best_path_length = 0;

		Ip->best_path_length = hop;

//		printf("the best path length is ------- %d\n",Ip->best_path_length);

		ptr = pp;

		for( i = Ip->best_path_length - 1 ; i >= 0 ; i--)
		{
			(Ip->best_path[i]) = ptr;

			(Ip->healing[i]) = ptr->healing;/*save the best heal values*/
	
//			printf("%s was added to the path in the %d position\n", ptr->name, i);

			ptr = ptr->prev;
		}

//		for(i = 0; i < Ip->best_path_length ;i++)
//		{
//			printf("%s\n", (Ip->best_path[i])->name);
//		}

	}
	
//	printf("the i_cpp is %lf\n",Ip->c_pp);
	//adjust the next jumps pp healed using the power reduction
	Ip->c_pp =  Ip->c_pp * (1 - (Ip->c_p_r)) ;

	num = (Ip->c_pp * (1 - Ip->c_p_r) );


//	printf("the new cpp added without rounding is %lf\n", num);

//	printf("the new pp healed w/rounding is %d \n",Ip->c_pp);

	
	/*go through the adajacent list of the given player and perform the DFS on each*/
	for(i = 0; i < (*pp).adj_size ; i++)
	{
		ptr = (*pp).adj[i];			/*Grab the pointer to this child*/

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

	//process command line arguments
	int i_rng = atoi(argv[1]);			//initial range--how far Urgosa can cast the spell initially (himself included) 
	int j_rng = atoi(argv[2]);			//jump range--how far the spell can jump from one player to the other
	int nm_j  = atoi(argv[3]);			//number of jumps--how many times the spell can jump
	int i_pwr = atoi(argv[4]);			//initial power--how much power the spell heals initially 
	double  pwr_r ;								//power range--how much the spell is reduced after each heal c_pwr-(1 - pwr_r)
	sscanf(argv[5],"%lf",&pwr_r);
	
	info *Ip = malloc(sizeof(info) );
	
//	Ip->i_rng = i_rng;
//	Ip->j_rng = j_rng;
	Ip->c_j = atoi(argv[3]);//nm_j;
	Ip->c_pp = atoi(argv[4]);//i_pwr;
	Ip->c_p_r = pwr_r;
	Ip->amnt_heal = 0;
	Ip->best_heal = 0;
	Ip->Init_pp = i_pwr;

	Ip->best_path = (player **) malloc( sizeof(player *) * atoi(argv[3]) );

	Ip->healing = (int *) malloc( sizeof( int * ) * atoi(argv[3]) );
	
//	printf("the i range-%d, jmp range-%d, number jmps-%d, i pwr-%d, pwr rd-%.2lf\n",i_rng, j_rng, nm_j, i_pwr, pwr_r);	
//	printf("the infos : i range-%d, jmp range-%d, number jmps-%d, pwr rd-%.2lf\n",Ip->i_rng, Ip->j_rng, Ip->c_j, Ip->c_p_r);	

	char hldr[100];

	int j,cnt, cast_size;

	int ma[3];
/*
	int * arp = (int *) malloc(sizeof(int)*3);

	printf("give me 3 intergers\n");

	scanf("%d %d %d",arp,arp+1,arp+2);


	printf("you game me %d %d %d\n", *arp, *(arp+1), *(arp+2) );
*/
//	double dist,xd,yd;

	double dist;

	int player_cnt, x, y, c_pp, mx_pp;

	int input = 1;


    player * pp, *p1, *p2;	

	player * prv = NULL;

	player * UP = NULL;;

	player_cnt = 0;

	cast_size = 0;

	//now take in lines from std input and create nodes for each line/player
	while( scanf("%d %d %d %d %s", &x, &y, &c_pp, &mx_pp, hldr) > 0 )
	{
//		printf("x:%d y:%d c_pp:%d m_pp:%d name;%s\n", x, y, c_pp, mx_pp, hldr);
		
		//allocate memory for a player and get the pointer to it
		//then initialize its values

		pp = (player *)malloc( sizeof(player) );

		pp->x = x;
		(*pp).y = y;
		(*pp).crnt_pp = c_pp;
		(*pp).max_pp = mx_pp;
		(*pp).visited = -1;
		(*pp).healing = 0;
		strcpy( (*pp).name, hldr);
		(*pp).prev = prv;
		
		/*check for Urgosa
		  if found save a pointer to it
		  Then set the current player as the previous player for linking purposes*/

		if( strcmp(hldr, "Urgosa_the_Healing_Shaman") == 0) UP = pp; 
		
		prv = pp;


		player_cnt++;

	}

//	printf("\nthe player count is %d\n", player_cnt );

	int i = 0;

	player *nodes[player_cnt];				/*will hold all of the nodes in the graph*/
	player *edges[player_cnt];				/*will be used as a temporary player pointer contianer*/
	player *ip[player_cnt];					/*will hold all the possible initial heal casts*/

	//now make the graph by going through the nodes using there previous fields
	//adding them to the graph array
	for( i=0 ; i < player_cnt ; i++)
	{
//		printf("the %d player is %s at (%d,%d), with %d mx pp and %d c pp\n",i,(char *) 
//				(*prv).name,(*prv).x,(*prv).y,(*prv).max_pp,(*prv).crnt_pp);
	
		nodes[i] = prv;
		prv = (*prv).prev;
	}

//	printf("\n\n");

	cnt = 0;
	int i_c_s = 0;

	//set up the possible initial casts from Urgosa by first getting the needed size of the array to be made
	//by looking for all players within the initial range. Those that meet the criteria are saved in ip
	for(i = 0; i < player_cnt; i++)
	{

			/*calculate the distance between Urgosa and all other nodes*/
			x = pow( ( (double) ( (*UP).x - (*nodes[i]).x ) ), 2 );
			y = pow( ( (double) ( (*UP).y - (*nodes[i]).y ) ), 2 );

			dist = ( sqrt(x + y) );

	//		printf("the distance to %s is %d\n",(*nodes[i]).name, (int)dist);

			/*If the distance is within the initial cast range store it in ip and increment cnt*/
			if(dist <= i_rng) 
			{
	//			printf("%s is in the initial cast range\n",(*nodes[i]).name);
				ip[i_c_s] = nodes[i];
				i_c_s++;
			}
	}

//	printf("The number of possible initial heals from  Urgosa is %d\n", i_c_s);

	//go through all nodes and set the adjacent lists based on jump range
	for( i = 0; i < player_cnt; i++)
	{
	//	printf("--------the %d element in the node array is %s \n", i , (*nodes[i]).name);

		cnt = 0;						/*cnt is used as the adjacent list size/counter*/

		//check the needed size of adjacent array for current node
		//by going through and counting how many are possible based on the distance from current node
		for(j = 0; j < player_cnt; j++)
		{
			//make sure not to count the node it self
			if(j != i)
			{
				/*calculate the distance*/
				
				x = pow( (double) ( (*nodes[i]).x - (*nodes[j]).x) ,2);
				y = pow( (double) ( (*nodes[i]).y - (*nodes[j]).y) ,2);
				dist = /*rint*/( sqrt( x + y) );

//				printf("the distance to %s is %lf ",(*nodes[j]).name, dist);

				/*check for those players that are within jump range if so add to edges temporarily and increment cnt*/
				
				if(dist <= j_rng) 
				{
//					printf("and it was added to %s's adjacent list\n",(*nodes[i]).name);
					edges[cnt] = nodes[j];	
					cnt++;
				}
//				else printf("and it was NOT ADDED!!!!!\n");
			}


		}

		/*From here down cnt now holds the number of edges that need to be added to the nodes[i].*/

//		printf("this node has %d edges\n",cnt);

		/*set the current nodes adjacent size field to cnt*/
		nodes[i]->adj_size = cnt;

		/*now set up edges to current node(nodes[i])*/

		//allocate enough space for cnt player pointers and set the pointer to it to this nodes adj
		nodes[i]->adj = (player **)malloc(sizeof(player *)* (cnt) );


		for(j = 0; j < cnt ; j++)
		{
	
			//add the pointer to the player pointer stored at the jth place in edges in to the adj pointer position offset by j
			nodes[i]->adj[j] = edges[j];

		}


	}

/*
	//Now perform a DFS on all possible initial heal nodes
	for(i = 0; i < i_c_s ; i++)
	{
		
		DFS( ip[i] , 1 , Ip, 0 ,NULL); 

	}
*/

	for(i = 0; i < player_cnt ; i++)
	{
		
//		calculate the distance between Urgosa and all other nodes
		x = pow( ( (double) ( (*UP).x - (*nodes[i]).x ) ), 2 );
		y = pow( ( (double) ( (*UP).y - (*nodes[i]).y ) ), 2 );

		dist = ( sqrt(x + y) );

		if(dist <= atoi(argv[1]) )
		{
			
			DFS( nodes[i] , 1 , Ip, 0 ,NULL); 
		}
	}

	for(i = 0; i < Ip->best_path_length ;i++)
	{
		printf("%s %d\n", (Ip->best_path[i])->name, Ip->healing[i] );
	}

	printf("Total_Healing %d\n", Ip->best_heal); 
	return 0;
}
