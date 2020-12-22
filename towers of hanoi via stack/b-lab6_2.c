// Vic Willis VAW42D, LaB 6, CS2050 Summer 2018

#include <stdio.h>
#include <stdlib.h>

/*
 *head is top of stack
 *next points to item behind it (eventually to the bottom)
 *prev points to item in front of it (eventually to the top)
*/

typedef struct Disk_S
{
	int disk_size;
	struct Disk_S *next, *prev;
} disk;

void push(disk **head, disk *new_disk);
disk *pop(disk **stack);
void move_stack(int level, disk **from, disk** to, disk **spare);
disk *init_game(int num_disks);
disk *return_tail(disk *stack);
void print_game(disk *head1, disk *head2, disk *head3, int num_disks);
void freeDisks(disk *head);

int main(int argc, char *argv[])
{
	// Error check command line
	if (argc != 2)
	{
		printf("Error: this program uses 2 commandline arguments.\n1: a.out\n2: a number of disks in the range of 3 to 9\n");
		return -1;
	}

	// Grab number of disks, error check if out of bounds
	int num_disks = atoi(argv[1]);
	if (num_disks < 3 || num_disks > 9)
	{
		printf("Error: the number of disks must be inclusively between 3 and 9!\n");
		return -1;
	}

	// Utility heads (to represent 3 separate stacks [rods]) 
	// game begins on stack 1
	disk *head1 = init_game(num_disks);
	disk *head2 = NULL;
	disk *head3 = NULL;

	// Print game
	print_game(head1, head2, head3, num_disks);
	
	// Test push / pop
	disk *new_disk = malloc(sizeof(disk));
	push(&head1, new_disk);
	head1 = pop(&head1);

	// Send each stack to be freed
	freeDisks(head1);
	freeDisks(head2);
	freeDisks(head3);

	return 0;
}

void push(disk **head, disk *new_disk)
{
	if (*head == NULL)
	{
		*head = new_disk;
		return;
	}
	else	
	{
	// Reassign head's prev from NULL to new disk (single deref since we have head's address)
	(*head)->prev = new_disk;
	// Assign new disk's next to the head
	new_disk->next = *head;

	// Assign new disk's prev to NULL
	*head = new_disk;
	// No need to return new "head" since we changed it's data by calling it by reference
	}
}

disk *pop(disk **head)
{
	// If removing from an empty list, do nothing and return the empty head
	if ((*head)->next == NULL)
	{
		return *head;
	}
	
	// Save the current head before we change it
	disk *ghost_head = *head;
	
	// The head becomes the node behind the one we're removing
	*head = ghost_head->next;

	// Detatch old head from the list
	ghost_head->next = NULL;

	if (*head != NULL)
	{
		(*head)->prev = NULL;
	}

	return ghost_head;
}

disk *return_tail(disk *stack)
{
	disk *temp = stack;
	if (temp != NULL)
	{
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
	}

	return temp;
}

disk *init_game(int num_disks)
{
	disk *head = NULL;
	
	int i;
	for (i = num_disks; i > 0; i--)
	{
		disk *new_disk = malloc(sizeof(disk));
		new_disk->disk_size = i;
		new_disk->next = NULL;
		new_disk->prev = NULL;
	
	push(&head, new_disk);
	}

	return head;
}

void print_game(disk *stack_one, disk *stack_two, disk* stack_three, int num_disks)
{
	// Point temp utilities to bottom of each stack
	disk *temp_one, *temp_two, *temp_three;
	temp_one = return_tail(stack_one);
	temp_two = return_tail(stack_two);
	temp_three = return_tail(stack_three);

	// Temporary string holders
	char arr_one[num_disks], arr_two[num_disks], arr_three[num_disks];

	// Fill arrays beginning w/ bottom of stack, remaining spaces get blanks
	int i;
	for (i=0; i<num_disks; i++)
	{
		if (temp_one != NULL)
		{
			arr_one[i] = temp_one->disk_size + '0';
			temp_one = temp_one->prev;
		}
		else
		{
			arr_one[i] = '-';
		}

		if (temp_two != NULL)
		{
			arr_two[i] = temp_two->disk_size + '0';
			temp_two = temp_two->prev;
		}
		else
		{
			arr_two[i] = '-';
		}

		if (temp_three != NULL)
		{
			arr_three[i] = temp_three->disk_size + '0';
			temp_three = temp_three->prev;
		}
		else{
			arr_three[i]='-';
		}
	}
	printf("\n");
	for (i=num_disks-1; i>=0; i--)
	{
		printf("  %c    %c    %c  \n", arr_one[i], arr_two[i], arr_three[i]);
	}

	printf("__________________\n");
}

void freeDisks(disk *head)
{
	// If stack is empty, do nothing and exit
	if (head == NULL)
	{
		return;
	}

	// Current is a utility to walk the list
	disk *current = head;

	// Dolphin will save current's "next" before it's freed
	disk *dolphin = current->next;

	while (current != NULL)
	{
		// Eventually current reaches bottom of stack, dolphin must be freed

		if (current->next == NULL)
		{
			free(dolphin);
			return;
		}

		
		free(current);

		// Current is refreshed to current->next
		current = dolphin;

		// Dolphin becomes the "next" of the refreshed Current
		dolphin = current->next;
	}
}
