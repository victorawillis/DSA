// Vic Willis VAW42D Lab7 CS2050 Summer 2018
#include <stdio.h>
#include <stdlib.h>

typedef struct Node_S
{
	int x, y;
	struct Node_S *left, *right;
} Node;

Node *importTree(char *fileName);
Node *addNode(Node *root, Node *newNode);
void inOrder(Node *root);
void preOrder(Node *root);
void freeTree(Node *root);
Node *deleteNode(Node *root, int key);

int main(int argc, char *argv[])
{
	// Command line error check, assign file name
	if (argc != 2)
	{
		printf("Error. This program needs 2 command line arguments.\n1: a.out\n2: <input file>\n");
		return -1;
	}

	char *fileName = argv[1];

	// Assemble the tree
	Node *root = importTree(fileName);

	// In-order print
	printf("\n");
	inOrder(root);
	printf("\n");

	// User to delete node based on it's "x" by entering a positive number
	int key, option = 1;
	do
	{
		printf("Enter a value to delete a node: ");
		scanf("%d", &key);
		while (key < 1)
		{
			printf("Error! Enter a positive value: ");
			scanf("%d", &key);
		}
	
		printf("To delete another node enter 1, to move on, enter 0: ");
		scanf("%d", &option);
		while (option < 0 ||  option > 1)
		{
			printf("Error! Enter a 1 or 0: ");
			scanf("%d", &option);
		}
	
		deleteNode(root, key);
	}
	while (option == 1);
	
	// Reprint after delete(s)
	printf("\nIn Order:\n");
	inOrder(root);
	printf("\nPre Order:\n");
	preOrder(root);
	printf("\n");

	// Free the tree
	freeTree(root);

	return 0;
}

Node *importTree(char *fileName)
{
	// Initially root is set to NULL to begin
	Node *root = NULL;

	FILE *fstream = fopen(fileName, "r");
	if (fstream == NULL)
	{
		printf("Error opening file.\n");
		return NULL;
	}

	// To store numbers in file
	int x, y;

	// Until we reach the end of the file, load each line into a new node
	while (fscanf(fstream, "%d %d", &x, &y) == 2)
	{
		// Error check malloc
		Node *newNode = malloc(sizeof(Node));
		if (!newNode)
		{
			printf("Memory allocation unsuccessful. Exiting.\n");
			exit(1);
		}

		// newNode is created as a leaf each time
		newNode->left = NULL;
		newNode->right = NULL;

		// Read file's numbers and assign them in newNode
	//	fscanf(fstream, "%d%d", &x, &y);
		newNode->x = x;
		newNode->y = y;

		// Add newNode to the tree
		root = addNode(root, newNode);
		free(newNode);
	}
	
	fclose(fstream);
	return root;
}

Node *addNode(Node *root, Node *newNode)
{
	// Executes if root is empty or when recursive base case reached
	if (root == NULL)
		return newNode;

	// Insert node into tree if its key is less than the root's
	if (newNode->x < root->x)
		root->left = addNode(root->left, newNode);

	// Insert node into tree if its keey is greater than the root's
	if (newNode->x > root->x)
		root->right = addNode(root->right, newNode);

	// When node finds appropriate position, return the updated root
	return root;
}

void inOrder(Node *root)
{
	if (root != NULL)
	{
		inOrder(root->left);
		printf("(%d, %d) -> ", root->x, root->y);
		inOrder(root->right);
	}
}

void preOrder(Node *root)
{
	if (root != NULL)
	{
		printf("(%d, %d) -> ", root->x, root->y);
		preOrder(root->left);
		preOrder(root->right);
	}
}

Node *leastValue(Node *root)
{
	// If root is empty...
	if (root == NULL)
		return root;
	// When left child is empty after recursion, we've found the minimum value
	if (root->left == NULL)
		return root;
	// Recur left as much as possible to find the minimum value
	return leastValue(root->left);
}

Node *deleteNode(Node *root, int key)
{
	// To save in case node to remove has children
	Node *pod;
	if (key < root->x)
	{
		root->left = deleteNode(root->left, key);
	}
	else
	{
		if (key > root->x)
		{
			root->right = deleteNode(root->right, key);
		}
		else
		{
			// If node has 2 children, save minimal value to maintain tree
			if (root->left && root->right)
			{
				pod = leastValue(root->right);
				root->x = pod->x;
				root->right = deleteNode(root->right, root->x);
			}
			else
			{
				pod = root;
				if (root->left == NULL)
				{
					root = root->right;
				}
				else
				{
					if (root->right == NULL)
					{
						root = root->left;
					}
					free(pod);
				}
			}
		}
	}
	
	return root;
}

void freeTree(Node *root)
{
	if (root != NULL)
	{
		freeTree(root->left);
		freeTree(root->right);
		free(root);
	}
}

