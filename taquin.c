#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h> //#include <malloc.h>
#include <math.h> //-lm pour compiler

#include "list.h"
#include "board.h"

list_t openList; // open list (noeuds qui vont être évaluer)
list_t closedList; // closed list (noeuds déjà évaluer)

void showSolution(Node goal)
{
	int i = 0;

	printf("\nSolution:");

	while (goal)
	{
		printBoard(goal);
		goal = goal->parent;
		i++;
	}

	printf("\nLength of the solution = %d\n", i - 1);
	printf("Size of open list = %d\n", openList.numElements);
	printf("Size of closed list = %d\n", closedList.numElements);
	return;
}

void UCS(void)
{
	Item *cur_node, *child_p, *temp;

	while (listCount(&openList))
	{
		cur_node = popBest(&openList);
		//si c'est le bon noeud
		if (evaluateBoard(cur_node) == 0.0)
		{
			showSolution(cur_node);
			return;
		}
		if (!onList(&closedList, cur_node->board))
		{
			addLast(&closedList, cur_node);
			for (int i = 0; i < MAX_BOARD; i++)
			{
				child_p = getChildBoard(cur_node, i);
				if (child_p != NULL)
				{

					temp = onList(&openList, child_p->board);
					if (temp)
					{

						if (temp->f > child_p->f)
						{
							delList(&openList, temp);
							addLast(&openList, child_p);
						}
					}
					if (!onList(&closedList, child_p->board))
					{
						addLast(&openList, child_p);
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
	while (listCount(&openList))
	{

		// sort le noeud avec f minimum de open list
		cur_node = popBest(&openList);
	
		// ajouter à closed list
		addLast(&closedList, cur_node);

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
			if (child_p != NULL && onList(&closedList, child_p->board)==NULL)
			{
				// vérifier s'il permet un plus court chemin sur un plateau
				temp = onList(&openList, child_p->board);
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
						delList(&openList, temp);
						addLast(&openList, child_p);
					}
				}
				// si child_p n'est pas dans open list 
				if (onList(&openList, child_p->board)==NULL)
				{
					// le mettre
					addLast(&openList, child_p);
				}
				
			}
		}
	}
}


int main()
{
	/* init lists */
	initList(&openList);
	initList(&closedList);

	// printList(openList);

	printf("\nInitial:");
	Node initial_state = initGame(2);
	printBoard(initial_state);

	printf("\nSearching ...\n");

	printList(openList);
	// printList(closedList);

	clock_t start, end;
	double cpu_time_used;

	start = clock();

	initial_state->f = 0.0;
	addLast(&openList, initial_state);
	// UCS();
	Aetoile();

	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Finished in %f seconds!\n", cpu_time_used);

	/* clean lists */
	cleanupList(&openList);
	cleanupList(&closedList);

	return 0;
}