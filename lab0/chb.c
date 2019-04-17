#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct plr
{
	int x;
	int y;
	int crnt_pp;
	int max_pp;
	char  name[100];
	struct plr * prev;
	int adj_size;
	struct plr **adj;
}player;


typedef struct information
{
	int i_rng;		//initial range
	int j_rng;		//jump range
	int c_j;		//current jump number
	double c_p_r;		//current power reduction
}info;

/*
player **set_edges(player * pp, int &min, int &cnt,player * nodes[] )
{
	player **p_pp;

	int i, j, k;

	player * adj = (player * )malloc(sizeof(player *)*cnt)

	//go through the nodes checking for players in possible reach to heal

	for(i = 0; i < cnt ; i++)
	{
		if(nodes[i] != pp)
		{
		   x = pow( ( (*pp).x - (*nodes[i]).x ) , 2 );
		   y = pow( ( (*pp).y - (*nodes[i]).y ) , 2 );

		   dist = sqrt(x + y);

			

		   if(dist <= min)
		   {
				
		   }

		}
	}

	return p_pp;
}
*/

int main( int argc, char** argv)
{

	//process command line arguments
	int i_rng = atoi(argv[1]);			//initial range--how far Urgosa can cast the spell initially (himself included) 
	int j_rng = (int)(*argv[2]) - '0';			//jump range--how far the spell can jump from one player to the other
	int nm_j  = (int)(*argv[3]) - '0';			//number of jumps--how many times the spell can jump
	int i_pwr = (int)(*argv[4]) - '0';			//initial power--how much power the spell heals initially 
	double  pwr_r ;								//power range--how much the spell is reduced after each heal c_pwr-(1 - pwr_r)
	sscanf(argv[5],"%lf",&pwr_r);
	
	info *Ip = malloc(sizeof(info) );
	
	Ip->i_rng = i_rng;
	Ip->j_rng = j_rng;
	Ip->c_j = nm_j;
	Ip->c_p_r = pwr_r;


	
	printf("the i range-%d, jmp range-%d, number jmps-%d, i pwr-%d, pwr rd-%.2lf\n",i_rng, j_rng, nm_j, i_pwr, pwr_r);	
	printf("the infos : i range-%d, jmp range-%d, number jmps-%d, pwr rd-%.2lf\n",Ip->i_rng, Ip->j_rng, Ip->c_j, Ip->c_p_r);	

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

	int dist;

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
		printf("x:%d y:%d c_pp:%d m_pp:%d name;%s\n", x, y, c_pp, mx_pp, hldr);
		
		//allocate memory for a player and get the pointer to it
		//then initialize its values

		pp = (player *)malloc( sizeof(player) );

		pp->x = x;
		(*pp).y = y;
		(*pp).crnt_pp = c_pp;
		(*pp).max_pp = mx_pp;
		
		strcpy( (*pp).name, hldr);
		(*pp).prev = prv;
		
		/*check for Urgosa
		  if found save a pointer to it
		  Then set the current player as the previous player for linking purposes*/

		if( strcmp(hldr, "Urgosa") == 0) UP = pp; 
		
		prv = pp;


		player_cnt++;

	}

	printf("\nthe player count is %d\n", player_cnt );

	int i = 0;

	player *nodes[player_cnt];				/*will hold all of the nodes in the graph*/
	player *edges[player_cnt];				/*will be used as a temporary player pointer contianer*/
	player *ip[player_cnt];					/*will hold all the possible initial heal casts*/

	//now make the graph by going through the nodes using there previous fields
	//adding them to the graph array
	for( i=0 ; i < player_cnt ; i++)
	{
		printf("the %d player is %s at (%d,%d), with %d mx pp and %d c pp\n",i,(char *) 
				(*prv).name,(*prv).x,(*prv).y,(*prv).max_pp,(*prv).crnt_pp);
	
		nodes[i] = prv;
		prv = (*prv).prev;
	}

	printf("\n\n");

	cnt = 0;

	//set up the possible initial casts from Urgosa by first getting the needed size of the array to be made
	//by looking for all players within the initial range. Those that meet the criteria are saved in ip
	for(i = 0; i < player_cnt; i++)
	{

			/*calculate the distance between Urgosa and all other nodes*/
			x = pow( ( (double) ( (*UP).x - (*nodes[i]).x ) ), 2 );
			y = pow( ( (double) ( (*UP).y - (*nodes[i]).y ) ), 2 );

			dist = rint( sqrt(x + y) );

			printf("the distance to %s is %d\n",(*nodes[i]).name, (int)dist);

			/*If the distance is within the initial cast range store it in ip and increment cnt*/
			if(dist <= i_rng) 
			{
				printf("\n",);
				ip[cnt] = nodes[i];
				cnt++;
			}
	}

	printf("The number of possible initial heals from  Urgosa is %d\n", cnt);

	//set up the size of the adjacent list
	//(*UP).adj_size = cnt;

	//go through all nodes and set the adjacent lists based on jump range
	for( i = 0; i < player_cnt; i++)
	{
		printf("--------the %d element in the node array is %s \n", i , (*nodes[i]).name);

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
				dist = rint( sqrt( x + y) );

				printf("the distance to %s is %d ",(*nodes[j]).name, dist);

				/*check for those players that are within jump range if so add to edges temporarily and increment cnt*/
				
				if(dist <= j_rng) 
				{
					printf("and it was added to %s's adjacent list\n");
					edges[cnt] = nodes[j];	
					cnt++;
				}
				else printf("and it was NOT ADDED!!!!!\n");
			}


		}

		/*From here down cnt now holds the number of edges that need to be added to the nodes[i].*/

		printf("this node has %d edges\n",cnt);

		/*set the current nodes adjacent size field to cnt*/
		nodes[i]->adj_size = cnt;

		/*now set up edges to current node(nodes[i])*/

		//pp = (player *)malloc(sizeof(player *)* (cnt+1) );
		//allocate enough space for cnt player pointers and set the pointer to it to this nodes adj
		nodes[i]->adj = (player **)malloc(sizeof(player *)* (cnt) );

		//nodes[i]->adj = &pp;
	

		//printf("nodes[i] = 0x%lx , adj = 0x%lx , pp = 0x%lx , &pp = 0x%lx\n", (unsigned long)nodes[i], (unsigned long)nodes[i]->adj
		//		  ,(unsigned long)pp, (unsigned long)&pp); 

		//printf("nodes[i] = 0x%lx , adj = 0x%lx , pp = 0x%lx , &pp = 0x%lx\n", (unsigned long)nodes[i], (unsigned long)nodes[i]->adj

		for(j = 0; j < cnt ; j++)
		{
		//	pp +=j;
		//	pp = edges[j];
			printf("the %d kid added is %s \n",j, (*edges[j]).name);
			//printf("the j is %d and the current kid is %s \n",j, (*edges[j]).name);
	
			//nodes[i]->adj[j] = edges[j];
			//add the pointer to the player pointer stored at the jth place in edges in to the adj pointer position offset by j
			nodes[i]->adj[j] = edges[j];
			
			//printf("nodes[i]->adj[j] = 0x%lx\n",(unsigned long)nodes[i]->adj[j]);
			//printf("pp = 0x%lx\n",(unsigned long)pp);
			//printf("*nodes[i]->adj = 0x%lx\n",(unsigned long)*nodes[i]->adj);
			//printf("*pp.name = %s\n",(*pp).name);
			//printf("(**nodes[i]->adj).name = %s\n", (**nodes[i]->adj).name );

		}

		printf("the current nodes or %s's kids are...\n", nodes[i]->name );

		/*the below will print out the current nodes adjacent list*/
		for(j = 0; j < cnt ; j++)
		{
			printf("%s\n",(*nodes[i]->adj[j]).name);
		}

		//pp +=1;
		//nodes[i]->adj[cnt] = NULL;

		//useing NULL as a sentinel for the end of the array
		//pp = NULL;

		//pp -= j+1;

		/*now set the current players adjacent pointer to the address of pp*/
		//nodes[i]->adj = &pp;

//		printf("the first name is %s\n", **(*nodes[i]).adj->name);

	}
/*
	for(i = 0; i < player_cnt; i++)
	{
		printf("----the %d element is %s and is kids are:\n", i, nodes[i]->name); 
		j = 0;

		while( pp != NULL)
		{
			pp = nodes[i]->adj[j];
printf("booobs\n");
			printf("the %d kid is %s\n",j ,pp->name );//pp->name);

			j++;
		}

	}
*/

	return 0;
}
