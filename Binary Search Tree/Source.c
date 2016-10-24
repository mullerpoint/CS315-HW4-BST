//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Gary Muller
// Fall 2016
// CS 315 Assignment 4
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A program that creates and utilizes a binary search tree, it will utilize a
// struct type def, and includes an add and remove functionality; it will utilize a
// stack to allow for traversals
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Header files standard libraries and classes
#include <stdio.h> //standard io functions for C
#include <stdlib.h> //used for better malloc() function
#include <ctype.h> //used for tolower() function
#include <stdbool.h> //used for bool types
#include <string.h> //string functions


//Gloabal Variables and Defines
//Structure Type Definition for a Binary Search Tree structure
typedef struct Binary_Tree_Item
{
	//node key
	char keyValue;
	//node data
	char payload[10];
	//pointer to child nodes
	struct Binary_Tree_Item *lItem, *rItem; //left and right subtree/items
}BST_ITEM;

//Structure Type Definition for a stack structure
typedef struct Stack_Item
{
	//key
	BST_ITEM *data;
	//pointer to child trees
	struct Stack_Item *next;
}STACK_ITEM;

//Function Prototypes
int addItem(BST_ITEM**, char, char*); //insert into tree
int inOrderTrav(BST_ITEM*); //in-order traverse the tree
int preOrderTrav(BST_ITEM*); //post-order traverse the tree
BST_ITEM* find(BST_ITEM*, char); //find the node with that key
int push(STACK_ITEM**, BST_ITEM*); //Push BST Node to stack
BST_ITEM* pop(STACK_ITEM**); //Pop BST Node from stack

//

//main driver function
main()
{
	//Variable Declarations
	char activity = ' ';
	char tempItemKey = ' ';
	char tempItemData[10];

	//BST Declaration

	BST_ITEM *ptrToBST_A = NULL;

	//loop for program body
	while (activity != 'q')
	{
		//query user for input on next action and normalize input
		printf("Enter choice (lower case is also acceptable) --- (I)nsert, (F)ind, (Q)uit: ");
		scanf(" %c", &activity);
		activity = tolower(activity); //make all inputs lowercase

		//use input as a case in switch

		switch (activity)
		{
		case 'i':
			//insert case
			//query user for data to add
			printf("Enter new node's key value: ");
			scanf(" %c", &tempItemKey);
			printf("\n");

			printf("Enter string of up to 10 characters for '%c's data : ", tempItemKey);
			scanf(" %[^\n]", tempItemData);
			printf("\n");

			//add to BST
			addItem(&(ptrToBST_A), toupper(tempItemKey), tempItemData);

			//Print traversals
			//preorder traversal
			printf("Pre-order traversal is: ");
			preOrderTrav(ptrToBST_A);
			printf("\n");

			//inorder traversal
			printf("Inorder traversal is: ");
			inOrderTrav(ptrToBST_A);

			printf("\n\n");

			break;

		case 'f':
			//find case

			//query user for data to find
			printf("Enter the search key: ");
			scanf(" %c", &tempItemKey);

			//find the data
			BST_ITEM *temp;
			temp = find(ptrToBST_A, toupper(tempItemKey));

			printf("Found the string '%s' there\n", temp->payload);

			break;

		case 'q':
			//quit case

			//quit gracefully
			printf("\n\nBye\n");

			break;

		default:
			printf("Please only use the specified inputs: i for insert, f for find, or q to quit\n\n");
			break;
		}//switch

		//clear the buffer of data for next loop
		memset(tempItemData, '\0', sizeof(tempItemData));
	}//while

}//main


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BST Functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//insert into tree
int addItem(BST_ITEM** treeRoot, char newKeyValue, char* newData)
{
	//set a variable to keep track of the root we are using
	BST_ITEM *currentRoot = *treeRoot;

	//build the node
	//get memory
	BST_ITEM *tempItem;
	tempItem = malloc(sizeof(BST_ITEM));
	//set values
	tempItem->keyValue = newKeyValue;
	strcpy(tempItem->payload, newData);
	tempItem->lItem = NULL;
	tempItem->rItem = NULL;

	//root is empty ie NULL
	if ((*treeRoot) == NULL)
	{
		(*treeRoot) = tempItem;
		//return success
		return 1;
	}

	//else the BST is not empty
	bool inserted = false; //the item has not been inserted

	while (!inserted)
	{
		//new key value is less than root
		if ((currentRoot->keyValue) > (tempItem->keyValue))
		{
			//left node is populated
			if (currentRoot->lItem != NULL)
			{
				currentRoot = currentRoot->lItem;
			}

			//left node is empty
			else
			{
				currentRoot->lItem = tempItem;
				inserted = true;
			}

		}//if

		//new key value is more than root
		else if ((currentRoot->keyValue) < (tempItem->keyValue))
		{
			//right node is populated
			if (currentRoot->rItem != NULL)
			{
				currentRoot = currentRoot->rItem;
			}

			//right node is empty
			else
			{
				currentRoot->rItem = tempItem;
				inserted = true;
			}

		}//elseif

		//new key value is equal to that of the root
		else
		{
			//return error
			return -1;
		}//else

	}//while
	return 1;
}

//in-order traverse the tree
int inOrderTrav(BST_ITEM* treeRoot)
{
	//variables
	bool done = false;
	BST_ITEM *currentRoot = treeRoot;
	STACK_ITEM *stackPtr = NULL;

	//traversal loop
	while (!done)
	{
		//traverse left
		if (currentRoot != NULL)
		{
			//save root for right traverse later
			push(&stackPtr, currentRoot);

			//traverse left
			currentRoot = currentRoot->lItem;
		}
		//traverse right
		else
		{
			//check that the stack is not empty
			if (stackPtr != NULL)
			{
				//backtrack
				currentRoot = pop(&stackPtr);
				//visit root
				printf("%c ", currentRoot->keyValue);

				//begin right traversal
				currentRoot = currentRoot->rItem;
			}
			//end the loop
			else
			{
				done = true;
			}//else
		}//else
	}//while

	//return success
	return 1;

}//inOrderTrav

//pre-order traverse the tree
int preOrderTrav(BST_ITEM* treeRoot)
{
	//variables
	BST_ITEM *currentRoot = treeRoot;
	STACK_ITEM *stackPtr = NULL;

	//check that the tree is not empty
	if (treeRoot == NULL)
	{
		//return error - theres no tree to traverse
		return -1;
	}

	//traversal loop
	while (true)
	{
		//move left and push to stack
		while (currentRoot != NULL)
		{
			//print root
			printf("%c ", currentRoot->keyValue);

			//push root to stack
			push(&stackPtr, currentRoot);

			//go left
			currentRoot = currentRoot->lItem;
		}//while

		//if root it null and stack is empty we've finished
		if ((currentRoot == NULL) && (stackPtr == NULL))
		{
			//return success
			return 1;
		}//if

		//pop the last node and move right
		currentRoot = pop(&stackPtr);
		currentRoot = currentRoot->rItem;


	}//while


	//return success
	return 1;

}//preOrderTrav

//find the node with that key
BST_ITEM* find(BST_ITEM* treeRoot, char seekKey)
{
	//if the tree is empty theres nothing to find
	if (treeRoot == NULL)
	{
		//return error
		return -1;
	}//if

	BST_ITEM *currentItem = treeRoot;

	//seek 
	while ((currentItem->keyValue) != seekKey)
	{
		//sought value is larger than current root
		if (seekKey > (currentItem->keyValue))
		{
			currentItem = currentItem->rItem;
		}
		//sought value is smaller than current root 
		else
		{
			currentItem = currentItem->lItem;
		}
	}


	return currentItem;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stack Functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//push BST node to stack
int push(STACK_ITEM** stackTop, BST_ITEM* node)
{
	//get new stack item
	STACK_ITEM *tempStackItem;
	tempStackItem = malloc(sizeof(STACK_ITEM));

	//set values
	tempStackItem->data = node;
	tempStackItem->next = *stackTop;

	//set the top to the new item
	*stackTop = tempStackItem;

	//return success
	return 1;
}//push

//pop BST node from stack
BST_ITEM* pop(STACK_ITEM** stackTop)
{
	if (*stackTop != NULL)//as long as the stack is not empty
	{
		STACK_ITEM *tempItem = *stackTop;
		*stackTop = (*stackTop)->next;

		return tempItem->data; //return the bst item not the stack item
	}//if
	else
	{
		return NULL;
	}

}//pop