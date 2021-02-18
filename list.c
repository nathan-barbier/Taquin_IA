#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

/* Item Functions */

Item *nodeAlloc()
{
	Item *node;

	node = (Item *)malloc(sizeof(Item));
	assert(node);

	node->parent = NULL;
	node->prev = NULL;
	node->next = NULL;
	node->f = node->g = node->h = (double)0.0;

	return node;
}



void freeItem(Item *node)
{
	if (node && node->board)
		free(node->board);
	if (node)
		free(node);
}

void initList(list_t *list_p)
{

	assert(list_p);

	list_p->numElements = 0;
	list_p->first = NULL;
	list_p->last = NULL;
}

int listCount(list_t *list)
{
	return list->numElements;
}

// return an item with corresponding board , or null
Item *onList(list_t *list, char *board)
{
	Item *pitem; // pointeur de noeud
	int i, diff;
	pitem = list->first; // pointe sur le 1er el de la liste
	if (list->first == NULL)
		return NULL;
	for(int j=0; j<list->numElements; j++) // tant que fin liste non atteinte
	{
		i = 0;
		diff = 0;

		// compare le board de l'item au board
		while (i < pitem->size && diff == 0)
		{
			// si different sort de la boucle
			if (pitem->board[i] != board[i])
			{
				diff = 1;
			}
			i++;
		}

		// si diff == 0 renvoyé pitem
		if (diff == 0)
			return pitem;
		pitem = pitem->next;
	}
	// si item non trouver
	return NULL;
}

// return and remove first item
Item *popFirst(list_t *list)
{
	Item *item = NULL;
	assert(list);				// assure que la liste est non vide
	item = list->first;			// sauvegarde le 1er el de la liste
	delList(list, list->first); // suppr le 1er el de la liste
	return item;
}

// return and remove last item
Item *popLast(list_t *list)
{
	Item *item = NULL;
	assert(list);			   // assure que la liste est non vide
	item = list->last;		   // sauvegarde le dernier el de la liste
	delList(list, list->last); // suppr le dernier el de la liste
	return item;
}

// remove a node from list
void delList(list_t *list, Item *node)
{

	if (node == (list->first))
	{							  // si le node à supprimmer est le 1er el de liste
		list->first = node->next; // faire pointer le 1er el de la liste sur le suivant

	}
	else if (node == (list->last))
	{							 //si le node à supprimmer est le dernier el de liste
		list->last = node->prev; //faire pointer le dernier el de la liste sur le precedent

	}
	if (node->prev != NULL)
	{
		(node->prev)->next = node->next;
	}
	if (node->next)
	{
		(node->next)->prev = node->prev;
	}
	// faire que le node pointe sur rien
	node->next = NULL;
	node->prev = NULL;
	// decrementer le nbr d'el de la liste
	(list->numElements)--;
}

// return and remove best item with minimal f value
Item *popBest(list_t *list) // and remove the best board from the list.
{
	assert(list);

	Item *list_min = list->first, *i = list->first;


	while (i->next)
	{
		i = i->next;
		if (list_min->f >= i->f)
			list_min = i;
	}

	delList(list, list_min);
	return list_min;
}

// add item in top
void addFirst(list_t *list, Item *node) // add in head
{
	//cas liste vide
	if ((list->first) == NULL)
	{
		// mettre le node en seul el de la liste
		list->first = node;
		list->last = node;
	}

	// cas liste non vide
	else
	{
		(list->first)->prev = node; // mettre le node devant la liste
		node->next = list->first;	// mettre le suivant de node sur le 1er el de la liste
		list->first = node;			// mettre node en 1er el de la liste
	}
	(list->numElements)++;
}

// add item in queue
void addLast(list_t *list, Item *node) // add in tail
{
	// cas liste vide
	if ((list->first) == NULL)
	{
		list->first = node;
		list->last = node;
	}

	// si liste non vide
	else
	{
		(list->last)->next = node; // contatener le node au dernier
		node->prev = list->last;   // mettre le precedent du node dernier el de ma liste
		list->last = node;		   // placer le pointeur vers le dernier el de la liste sur node
	}
	(list->numElements)++; // incrementer le nombre de el de la liste
}

// fonction qui nettoie une liste
void cleanupList(list_t *list)
{
	Item *delnode = (list->first);					 // pointer le node à suppr premier el de la liste
	Item *psuiv;									 // el suivant
	while (delnode != NULL && delnode->next != NULL) // tant que l'on a pas atteint le dernier el
	{
		psuiv = delnode->next;	// poser le suivant
		delList(list, delnode); // supprimer le node
		freeItem(delnode);
		delnode = psuiv; // mettre le suivant en el à suppr
	}
	// reinitialiser les valeurs de la liste
	list->first = NULL;
	list->last = NULL;
	list->numElements = 0;
}

void printList(list_t list)
{
	Item *item = list.first;
	while (item)
	{
		printf("%.2f [%s] - ", item->f, item->board);
		item = item->next;
	}
	printf(" (nb_items: %d)\n", list.numElements);
}


// TEST LIST
//gcc list.c -o list
/*
int main()
{
	printf("tests\n");

  	Item *item;
	char str[3];

	list_t openList;

	printf("variable init\n");
  
	initList( &openList );

	for (int i=0; i<10; i++) {
		item = nodeAlloc();
		item->f = i;
		sprintf(str, "%2d", i);
        item->board = strdup(str);
		//printf("addlast\n");
		addLast( &openList, item );
	}

	printf("addlast done\n");

	for (int i=20; i<25; i++) {
    item = nodeAlloc();
    item->f = i;
    sprintf(str, "%2d", i);
    item->board = strdup(str);
    addFirst( &openList, item );
  	}	

	printf("addfirst done\n");

	printList(openList);
	printf("\n");


	Item *node = popBest( &openList );
	printf("best node = %.2f\n", node->f);
	printList(openList);
	printf("\n");
	

	strcpy(str, "23");
	node = onList(&openList, str);
	if (node) printf("found %s: %.2f!\n", str, node->f); 
	printList(openList);
	printf("\n");

	node = popFirst(&openList);
	
	item = nodeAlloc();
    item->f = 50;
    sprintf(str, "50");
    item->board = strdup(str);
    addLast( &openList, item );

	node = popFirst(&openList);
	if (node) printf("first: %.2f\n", node->f); 
	printList(openList);
    printf("\n");

	node = popLast(&openList);
	if (node) printf("last: %.2f\n", node->f); 
	printList(openList);
    printf("\n");

	printf("clean\n");	
	cleanupList(&openList);
	printf("clean done\n");
	printList(openList);
	printf("\n");
  
	return 0;
}
*/
/*
int main()
{
	printf("hello world");

	return 0;
}
*/
