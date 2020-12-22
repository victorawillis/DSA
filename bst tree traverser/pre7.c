#include <stdio.h>
#include <stdlib.h>

typedef struct Number_S
{
	int n;
	struct Number_S *left, *right;
} Number;

int menu(void);
Number *addNode(Number *root, int n);
//
//

int main(int argc, char *argv[])
{
	// Get user selection
	int option = menu();

	// Create initial root and value variable
	Number *root = NULL;
	int n;

	// Open file containing values
	FILE *fstream = fopen(argv[1], "r");
	if (fstream == NULL)
	{
		printf("ERROR!!! ERRORRRR!!!!!\n\n\n.......error!!!\n");
		return -1;
	}

	// Allow continuous run
	while (option != 4)
	{
		switch (option)
		{
			case 1:
				fscanf(fstream, "%d", &n);
				root = addNode(root, n);
			break;

			case 2:
	
			break;

			case 3:

			break;
			
			case 4:
				// Quit
			return 0;
		}
		
		option = menu();
	}

	return 0;
}

int menu(void)
{
	int option;
	printf("Make a choice...");
	printf("\n1: Add number\n2: Remove number\n3: Print numbers\n4: Quit\n");
	scanf("%d", &option);
	
	while (option < 1 || option > 4)
	{
		printf("Error: try a value between 1 and 4\n");
		scanf("%d", &option);
	}

	return option;
}
	
Number *addNode(Number *pod, int n)
{
	// Catches empty tree, also finds child after recursion
	if (pod == NULL)
	{
		malloc(sizeof(Number));
		pod->n = n;
		pod->left = NULL;
		pod->right = NULL;
		
		return pod;
	}

	// If n < pod key, keep going left until we hit child
	if (n < pod->n)
	{
		pod->left = addNode(pod->left, n);
	}

	// If n > pod key, keep going right until we hit child
	else if (n > pod->n)
	{
		pod->right = addNode(pod->right, n);
	}

	return pod;
}
