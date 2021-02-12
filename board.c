#include <stdio.h>
//#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "board.h"

#include <string.h>

// Initialize a new Game for the nQueens problem: an empty board..
Node initGame(int choice)
{
	int i, temp;
	Node node;
	char *diff;
	char easy[MAX_BOARD] = {1, 2, 5, 3, 0, 4, 7, 8, 6};
	char moy[MAX_BOARD] = {7, 4, 8, 2, 5, 6, 3, 1, 0};
	char hard[MAX_BOARD] = {8, 0, 7, 5, 6, 1, 3, 2, 4};

	switch (choice)
	{
	case 1:
		diff = easy;
		break;

	case 2:
		diff = moy;
		break;

	case 3:
		diff = hard;
		break;

	default:
		diff = hard;
		break;
	}

	char *initial = (char *)malloc(MAX_BOARD * sizeof(char));
	for (int i = 0; i < MAX_BOARD; i++)
	{
		initial[i] = diff[i];
		if (initial[i] == 0)
			temp = i;
	}

	node = nodeAlloc();

	initBoard(node, initial);

	node->depth = 0;
	node->blank = temp;

	return node;
}

// print a board
void printBoard(Node node)
{
	assert(node);

	printf("\n");
	for (int j = 0; j < WH_BOARD; j++)
		if (j == 0)
			printf(" ___");
		else
			printf("____");
	printf("\n");
	for (int i = 0; i < MAX_BOARD; i++)
	{
		if (i % WH_BOARD == 0)
			printf("|");
		if (node->board[i] == 0)
			printf("   |");
		else
			printf("%2d |", node->board[i]);
		if (((i + 1) % WH_BOARD) == 0)
		{
			printf("\n");
			for (int j = 0; j < WH_BOARD; j++)
				if (j == 0)
					printf(" ___");
				else
					printf("____");
			printf("\n");
		}
	}
	printf("\n");
}

// initialize node's state from a given board
void initBoard(Node node, char *board)
{
	assert(node);

	node->size = MAX_BOARD;
	node->board = calloc(MAX_BOARD, sizeof(char));

	/* Copy board */

	for (int i = 0; i < MAX_BOARD; i++)
	{
		node->board[i] = board[i];
	}
}

// Return 0 if all queens are placed. Positive otherwise
// Ie: nb queens that still need to be placed.
double evaluateBoard(Node node)
{
	int cost = 9;
	char check[MAX_BOARD] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
	char *board = node->board;

	for (int i = 0; i < MAX_BOARD; i++)
	{
		if (board[i] == check[i])
			cost = cost - 1;
	}
	return cost;
}

// Test if position pos is valid with respect to node's state

int isValidPosition(Node node, int pos)
{
#define MAX_VECTOR 4
	typedef struct
	{
		int len;						//nd de mouv possible
		unsigned int valid[MAX_VECTOR]; //case ou on peut se déplacer
	} move_t;

	const move_t moves[MAX_BOARD] = {
		/* 0 */ {2, {1, 3}},
		/* 1 */ {3, {0, 2, 4}},
		/* 2 */ {2, {1, 5}},
		/* 3 */ {3, {0, 4, 6}},
		/* 4 */ {4, {1, 3, 5, 7}},
		/* 5 */ {3, {2, 4, 8}},
		/* 6 */ {2, {3, 7}},
		/* 7 */ {3, {4, 6, 8}},
		/* 8 */ {2, {5, 7}}};

	//position du blanc
	int posblank = node->blank;
	for (int i = 0; i < moves[posblank].len; i++)
	{
		if (pos == moves[posblank].valid[i])
			return 1;
	}
	return 0;
}

// Return a new item where a new queen is added at position pos if possible. NULL if not valid
Node getChildBoard(Node node, int pos)
{
	int temp;
	Node child_p = NULL;

	if (isValidPosition(node, pos))
	{
		/* allocate and init child node */
		child_p = nodeAlloc();
		initBoard(child_p, node->board);

		/* Make move (On place une nouvelle reine)*/
		// Est ce qu'on doit vérifier avec evaluateBoard si les reines sont déjà placées ?
		temp = child_p->board[pos];
		child_p->board[pos] = 0;
		child_p->board[node->blank] = temp;

		child_p->blank = pos;
		/* link child to parent for backtrack */
		child_p->parent = node;

		child_p->depth = (child_p->parent)->depth + 1;
		//calcul coût du noeud
		child_p->g = child_p->depth;
		
		
		child_p->f = (child_p->g) + (child_p->h);
	}

	return child_p;
}
