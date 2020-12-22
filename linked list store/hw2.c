// Vic Willis VAW42D, Homework 2, CS2050, Summer 2018

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define length 16

typedef struct Bank_S
{
	int account;
	char *name;
	float amount;
	struct Bank_S *next;
} Bank;

Bank *createNode(char *name, int account, float amount); 
Bank *insertNode(Bank *head, Bank *node);
Bank *buildList(char *fileName);
int checkList(Bank *head, char *name, int account, float amount);
Bank *updateList(char *fileName, Bank* head);
void printList(Bank *head);
Bank *deleteNode(Bank *head);
int writeOutput(char *fileName, Bank *head);
void freeNodes(Bank *head);

int main(int argc, char *argv[])
{
	// Error check command line
	if (argc != 4)
	{
		printf("Error: this program requires 4 command line arguments.\n");
		printf("1: a.out\n2: account.txt\n3: update.txt\n4: output.txt\n");
		return -1;
	}

	// Save account file names
	char *fileName1 = argv[1];
	char *fileName2 = argv[2];
	char *fileName3 = argv[3];

	// Building the list; main is given access to the original head
	Bank *head;
	head = buildList(fileName1);

	// Prints
	printf("\n\nOriginal list\n\n");
	printList(head);

	printf("\n\nUpdated list\n\n");
	Bank *head2 = updateList(fileName2, head);
	printList(head2);

	printf("\n\nList, after removing users who are in debt\n\n");
	Bank *head3 = deleteNode(head2);
	printList(head3);
	printf("\n");

	// Write final list to file
	writeOutput(fileName3, head3);
	
	// Free the list
	freeNodes(head3);

	return 0;
}

Bank *createNode(char *name, int account, float amount)
{
	// Make node
	Bank *newNode = malloc(sizeof(Bank));
	newNode->name = malloc(sizeof(char)*length);

	// Assign values
	strcpy(newNode->name, name);
	newNode->account = account;
	newNode->amount = amount;
	newNode->next = NULL;

	return newNode;	
}

Bank *insertNode(Bank *head, Bank *node)
{
	// Utilities to keep track of previous and current nodes for traversing the list
	Bank *current = head;
	
	// Walk through list until we hit the end while keeping track of the previous node
	while (current->next != NULL)
	{
		current = current->next;
	}
	// Loop exits when current is at the last element of the list
	
	// The previous tail will now point to new tail, which points to NULL
	current->next = node;
	
	return head;
}

Bank *buildList(char *fileName)
{
	// Open file
	FILE *fstream = fopen(fileName, "r");
	if (fstream == NULL)
	{
		printf("Error: file open was unsuccessful.\n");
		return NULL;
	}

	// Read number of lines at top of file
	int lines;
	fscanf(fstream, "%d", &lines);
	
	// Declare head and variably new node
	Bank *head = NULL;
	Bank *newNode = malloc(sizeof(Bank));

	// Read line, send data to createNode, then send the node to buildList
	int account;
	char name[length];
	float amount;

	// Assign head
	fscanf(fstream, "%d %s %f \n", &account, name, &amount);
	head = createNode(name, account, amount);

	int i;
	for (i=1; i<lines; i++)
	{
		// Store line from file
		fscanf(fstream, "%d %s %f \n", &account, name, &amount);

			// New node will be created
			newNode = createNode(name, account, amount);
			// Insert node
			insertNode(head, newNode);
	}
	
	fclose(fstream);
	return head;
}

int checkList(Bank *head, char *name, int account, float amount)
{
	// Walk through list
	Bank *current = head;
	while (current->next != NULL)
	{
		// Compare data sent to this function (from update.txt) to the list (account.txt)
		if (current->account == account)
		{
			current->amount += amount;
			return 1;
		}

		current = current->next;
	}

	// Check the last node in list
	if (current->account == account)
	{
		current->amount += amount;
		return 1;
	}
	
	printf("Error: account not found.\n");
	return 0;
}

Bank *updateList(char *fileName, Bank *head)
{
	// Open file (update.txt)
	FILE *fstream = fopen(fileName, "r");
	if (fstream == NULL)
	{
		printf("Error: file open was unsucessful.\n");
		return NULL;
	}
	
	// Read number of lines at top of file
	int lines;
	fscanf(fstream, "%d", &lines);

	int account2;
	char name2[length];
	float amount2;

	int i;
	for (i=0; i<lines; i++)
	{
		// Store data from each node in the list
		fscanf(fstream, "%d %s %f", &account2, name2, &amount2);

		// Call checkList
		int check = checkList(head, name2, account2, amount2);
		
		if (check == 0)
		{
			Bank *newNode = createNode(name2, account2, amount2);
			insertNode(head, newNode);
		}
	}
	
	fclose(fstream);
	return head;
}

void printList(Bank *head)
{
	// Utility tracker node
	Bank *current = head;
	
	// Walk through list until we reach last node
	while (current->next != NULL)
	{
		printf("%d %s %.2f\n", current->account, current->name, current->amount);
		current = current->next;
	}

	// Last node reached but did not print in loop; print it
	printf("%d %s %.2f\n", current->account, current->name, current->amount);
}

Bank *deleteNode(Bank *head)
{
	// Walk through updated list, and keep track of previous node
	Bank *previous = NULL;
	Bank *current = head;

	while (current->next != NULL)
	{
		if (current->amount >= 0)
		{
			// Remove nothing, simply update utility nodes
			previous = current;
			current = current->next;
		}

		if (current->amount < 0)
		{
			// Case if removing first node
			if (previous == NULL)
			{
				// Set head equal to current's "next"
				head = current->next;
				// Free current name & node itself
				free(current->name);
				free(current);
				// Set current equal to the head, as it's now at beginning
				current = head;
			}

			else
			{
				// Replace previous node's "next" with current's "next"
				previous->next = current->next;

				// Remove current node
				free(current->name);
				free(current);

				// Recreate "current" for the loop to reference
				current = previous->next;
			}
		}
	}

	// Case for removing tail node
	if (current->amount < 0)
	{
		previous->next = NULL;
		free(current->name);
		free(current);
	}

	return head;
}

int writeOutput(char *fileName, Bank *head)
{
	FILE *fstream = fopen(fileName, "w");
	if (fstream == NULL)
	{
		printf("Error: file open was uncessesful.\n");
		return 0;
	}

	// Walk through list to file print it to our output file
	Bank *current = head;
	while (current->next != NULL)
	{
		fprintf(fstream, "%d %s %.2f\n", current->account, current->name, current->amount);
		current = current->next;
	}

	// Write last node to file
	fprintf(fstream, "%d %s %.2f", current->account, current->name, current->amount);

	printf("File successfully written.\n");
	return 1;
}

void freeNodes(Bank *head)
{
	// Walk through list, free names & nodes themselves
	Bank *current = head;
	Bank *saveNext;
	while (current->next != NULL)
	{
		saveNext = current->next;
		free(current->name);
		free(current);

		current = saveNext;
	}

	// Exit list; free last node
		free(current->name);
		free(current);
}			
