// Vic Willis - bst store

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCITYNAME 30
#define MAXLINELENGTH 50

typedef struct n_
{
	int zipCode;
	char *city;
	char state[3];
	struct n_ *left, *right;
} Node;

Node *importTree(char *fileName);
int menu(void);
Node *addNode(Node *root, Node *newNode);
int findStateCount(Node *root, char *state);
Node *findZipCode(Node *root, int zipCode);
void freeTree(Node *root);
void testPrint(Node *root);

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Error. This program accepts 2 command line arguments.\n1: a.out\n2: <filename>\n");
		return -1;
	}

	// Open file, give data to importTree which assigns the nodes into a BST
	Node *root = importTree(argv[1]);
	
	// The user can input a State to search, or a Zip Code to search
	char state[3];
	int zipCode;

	// Menu screen
	int option = menu();
	while (option != 3)
	{
		switch (option)
		{
			case 1:
				printf("Enter state: ");
				scanf("%s", state);
				printf("Zipcodes in state: %d\n", findStateCount(root, state));
			break;
	
			case 2:
				printf("Enter a zip code to search for: ");
				scanf("%d", &zipCode);
				Node *pod = findZipCode(root, zipCode);
				if (pod != NULL)
				{
					printf("	Search result for: \"%d\"\n	City: %s\n	State: %s\n", pod->zipCode, pod->city, pod->state);
				}
				else
				{
					printf("	No search result for: \"%d\"\n", zipCode);
				}	
			break;
		}

		option = menu();
	}
	
	// Free entire tree
	freeTree(root);

	// Exiting statements
	printf("\nTerminating program.\n");
	//free(root);
	return 0;
}

int menu(void)
{
	// Get menu option from user with error check
	int option;
	printf("1: Find number of zipcodes in a state\n2: Find the zip\n3: Exit\n");
	scanf("%d", &option);
	while (option < 1 || option > 3)
	{
		 
		
		printf("Error. Enter a 1, 2, or 3: ");
		scanf("%d", &option);
	}

	return option;
}

Node *addNode(Node *root, Node *newNode)
{
	// Executes if root is empty, or when recursive base case is reached
	if (root == NULL)
	{
		return newNode;
	}
	
	// Insertion, given the zip is lesser than the root's
	if (newNode->zipCode < root->zipCode)
	{
		// Recur left until the new zip is greater than its parent
		root->left = addNode(root->left, newNode);
	}

	// Insertion, given the zip is greater than the root's
	if (newNode->zipCode > root->zipCode)
	{
		// Recur right until new zip is less than its parent
		root->right = addNode(root->right, newNode);
	}

	return root;
}

int findStateCount(Node *root, char *state)
{
	// Executes if list is empty or when recursion reaches the end of a leaf
	if (root == NULL)
		return 0;

	// Count will sum the times strcmp finds a match in a State between the node it's on and the user input
	int count = 0;
	count += findStateCount(root->left, state);
	count += findStateCount(root->right, state);

	// This returns the 1 + the amount of times a match was found
	if (strcmp(root->state, state) == 0)
		return ++count;

	// If the no match, simply return the count to keep track of it, but don't add to it
	return count;

}

Node *findZipCode(Node *root, int zipCode)
{
	// Executes if the list is empty or when recursion reaches the end of a leaf, or when a match is found!
	if (root == NULL)
		return root;

	// When a node's zip matches the input zip, print the result, return a null so the 1st if(...) can exit the function
	if (root->zipCode == zipCode)
	{
		//printf("Result for %d\nCity: %s\nState: %s\n", zipCode, root->city, root->state);
		return root;
	}

	// When the node's zip is less than the input zip, recur right - else, recur left
	if (root->zipCode < zipCode)
		return findZipCode(root->right, zipCode);
	else
	{
		return findZipCode(root->left, zipCode);
	}
}

void freeTree(Node *root)
{
	if (root != NULL)
	{
		freeTree(root->left);
		free(root->city);
		free(root);
		freeTree(root->right);
	}
}

Node *importTree(char *fileName)
{
	// Root is not passed to this function, it must be declared here, leaves will be added to it in the while loop below
	Node *root = NULL;
	// Declare file pointer, open <file> in read-mode
	FILE *fp = fopen(fileName, "r");
	// Same as testing if the pointer "fp" is NULL - FILE pointers return NULL upon failed open
	if (!fp)
	{
		printf("Error opening file.\n");
		return NULL;
	}
	// feof(FILE *) returns 0 UNTIL the end of the file is reached (is returning a non-zero value each loop iteration) - until then, keep looping
	while (!feof(fp))
	{
		// new is a utility, it's given a random address in RAM of size Node
		Node *new = malloc(sizeof(Node));
		// If new is of NULL value, malloc failed
		if (!new)
		{
			printf("Failed to allocate memory for node. Ending read.\n");
			exit(1);
		}
		// new's member "city" is a string of chracters - this pointer-array needs space in RAM too
		new->city = malloc(sizeof(char)*MAXCITYNAME);
		// if "city" is of NULL value, malloc failed
		if (!(new->city))
		{
			printf("Failed to allocate memory for city name. Ending read.\n");
			exit(1);
		}
		// new will be assigned to the tree as a leaf each iteration, so it will not have children
		new->left = NULL;
		new->right = NULL;
		// line is a pointer array of characters - each "line" in the file will be stored here for parsing later
		char *line = malloc(sizeof(char)*MAXLINELENGTH);
		// If line is of NULL value, malloc failed
		if (!line)
		{
			printf("Failed to allocate memory for line. Ending read.\n");
			exit(1);
		}
		// fgets(char*, int, FILE *) stores characters from a file into arg1 until the it reads: "\n", (arg2 - 1) number of characters, or EOF
		// fgets(...) returns NULL if the read fails or if it reads nothing before EOF
		if (fgets(line, MAXLINELENGTH, fp) == NULL)
		{
			// Exit if file read fails during the call of fgets
			if (!feof(fp))
			{
				printf("File reading ended prematurely. Check for errors in the file.\n");
			exit(1);
			}
			// On the last attempted read, the conditional will recieve NULL (EOF), so free the utility "new" and close the stream
			free(new->city);
			free(line);
			free(new);
			fclose(fp);
			break;
		}
		// Temp is a utility to store tokens, defined here as the characters between commas
		char *tmp = strtok(line, ",");
		// The first token is a string of characters that make up the Zip Code, so will be converted to integers and assigned to the new node's zipCode
		new->zipCode = atoi(tmp);
		// The second token is the city name
		tmp = strtok(NULL, ",");
		// Copy the token into the new node's member "city"
		strcpy(new->city, tmp);
		// strcpy(char*, char*) doesn't add a null terminator so it must be done manually
		new->city[strlen(tmp)+1] = '\0';
		// The third token is the state
		tmp = strtok(NULL, ",");
		// Copy the state into the node's member "state"
		strcpy(new->state, tmp);
		// Again, manually add null terminator
		new->state[2] = '\0';
		// Reassign the root so that the tree's new leaf is included
		root = addNode(root, new);
		// If the root is NULL at this point, the new node was never correctly added to the tree
		if (!root)
		{
			printf("Root of tree is still NULL! Ending read.\n");
			exit(1);
		}
		// With each iteration, "line" will be reallocated, used to declare the newest node, and freed
		free(line);
		// No need to free "city" or "state" here because their allocated space belongs to their node's members
	}
	// Finally after declaring each node and place it along the tree, returning "root" will allow this function to send the finished tree's root to main!
	return root;
}
