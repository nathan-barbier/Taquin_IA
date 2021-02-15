#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h> //#include <malloc.h>
#include <math.h> //-lm pour compiler

#include "list.h"
#include "board.h"

list_t frontier; // open list (noeuds qui vont être évaluer)
list_t explored; // closed list (noeuds déjà évaluer)

void showSolution(Node goal)
{
	int i = 0,
		j;

	printf("\nSolution:");

	while (goal)
	{
		printBoard(goal);
		goal = goal->parent;
		i++;
	}

	printf("\nLength of the solution = %d\n", i - 1);
	printf("Size of open list = %d\n", frontier.numElements);
	printf("Size of closed list = %d\n", explored.numElements);
	return;
}

void UCS(void)
{
	Item *cur_node, *child_p, *temp;
	int i;

	while (listCount(&frontier))
	{
		cur_node = popBest(&frontier);
		//si c'est le bon noeud
		if (evaluateBoard(cur_node) == 0.0)
		{
			showSolution(cur_node);
			return;
		}
		if (!onList(&explored, cur_node->board))
		{
			addLast(&explored, cur_node);
			for (int i = 0; i < MAX_BOARD; i++)
			{
				child_p = getChildBoard(cur_node, i);
				if (child_p != NULL)
				{

					temp = onList(&frontier, child_p->board);
					if (temp)
					{

						if (temp->f > child_p->f)
						{
							delList(&frontier, temp);
							addLast(&frontier, child_p);
						}
					}
					if (!onList(&explored, child_p->board))
					{
						addLast(&frontier, child_p);
					}

				}
			}
		}
	}
}

//calcul le h d'un noeud
//simple nb de cases mal placées
//retourne si noeud sol
double getsimpleh(Item *node)
{
	double h = 9.0;
	char check[MAX_BOARD] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
	char *board = node->board;

	for (int i = 0; i < MAX_BOARD; i++)
	{
		if (board[i] == check[i])
			h = h - 1.0;
	}
	return h;
}

//somme des distances des cases par rapport  la sol
//0 si sol
double getManhatanh(Item *node)
{
	double h = 0;
	// char check[MAX_BOARD]={1, 2, 3, 4, 5, 6, 7, 8, 0};
	char *board = node->board;

	// pour toutes les cases du plateau
	for (int i = 0; i < MAX_BOARD; i++)
	{
		int j;
		// indique case correspondante dans la solution
		if (board[i] == 0)
			j = 8;
		else
			j = board[i] - 1;

		// coord de la case
		double x1 = (i / WH_BOARD);
		double y1 = (i % WH_BOARD);

		// coord de la solution
		double x2 = (j / WH_BOARD);
		double y2 = (j % WH_BOARD);

		double x = x1 - x2;
		double y = y1 - y2;

		// calcul distance
		double dist = sqrt(pow(x, 2) + pow(y, 2));
		h = h + dist;
	}

	return h;
}

//comme UCS mais calcul du cout change avec h en plus
void Aetoile(void)
{
	Item *cur_node, *child_p, *temp;
	// cur_node : noeud évalué

	//tant qu'il reste des noeuds à évaluer
	while (listCount(&frontier))
	{

		// sort le noeud avec f minimum de open list
		cur_node = popBest(&frontier);
	
		// ajouter à closed list
		addLast(&explored, cur_node);

		//si c'est le bon noeud
		if (evaluateBoard(cur_node) == 0)
		{
			showSolution(cur_node);
			return;
		}


		// pour tous les voisins de cur_node
		for (int i = 0; i < MAX_BOARD; i++)
		{
			child_p = getChildBoard(cur_node, i);
			// si il est possible et non exploré
			if (child_p != NULL && onList(&explored, child_p->board)==NULL)
			{
				// vérifier s'il permet un plus court chemin sur un plateau
				temp = onList(&frontier, child_p->board);
				//calcul de l'heuristique
				child_p->h=getsimpleh(child_p);
				//child_p->h = getManhatanh(child_p);
				// calcul coût totale (g est la pronfondeur nb de mouvements)
				child_p->f = (child_p->g) + (child_p->h);
				if (temp)
				{
					// si le chemin est plus court
					if (temp->f > child_p->f)
					{
						// remplacer temp par child_p
						delList(&frontier, temp);
						addLast(&frontier, child_p);
					}
				}
				// si child_p n'est pas dans open list 
				if (onList(&frontier, child_p->board)==NULL)
				{
					// le mettre
					addLast(&frontier, child_p);
				}
				
			}
		}
	}
}


int main()
{
	/* init lists */
	initList(&frontier);
	initList(&explored);

	// printList(frontier);

	printf("\nInitial:");
	Node initial_state = initGame(1);
	printBoard(initial_state);

	printf("\nSearching ...\n");

	printList(frontier);
	// printList(explored);

	initial_state->f = 0.0;
	addLast(&frontier, initial_state);
	//UCS();
	Aetoile();
	printf("Finished!\n");

	/* clean lists */
	cleanupList(&frontier);
	cleanupList(&explored);

	return 0;
}