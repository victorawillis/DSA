// VAW42D - Vic Willis - Homework 2 - Section H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 6
#define COL 4
#define MAX 25

// Function prototypes
void initialSeats(char flight[ROW][COL], int count);
void displayMenu(void);
void printFlightMap(char flight[ROW][COL]);
int loginMatch(int passcode, int adminPasscode);
int getTotalRevenue(char flight1[ROW][COL], char flight2[ROW][COL], char flight3[ROW][COL]);
void flightMenu(void);
void seatReservation(char flight[ROW][COL]);
void printMessage(char name[], char flight_number[]);

// Seat pricing
int costMatrix[ROW][COL]={{500, 200, 200, 500},
			  {500, 200, 200, 500},
		  	  {500, 200, 200, 500},
			  {500, 200, 200, 500},
			  {500, 200, 200, 500},
			  {500, 200, 200, 500}};

// Main
int main(void)
{
	// Initial seats
	srand(time(NULL)); int seats=rand()%5+1;
	char flight1[ROW][COL], flight2[ROW][COL], flight3[ROW][COL];
	initialSeats(flight1, seats);
	initialSeats(flight2, seats);
	initialSeats(flight3, seats);

	// Display main menu
	int menuOption;
	displayMenu();
	scanf("%d", &menuOption);
	while (menuOption<1||menuOption>3)
	{
		printf("Error: invalid option. Try again: ");
		scanf("%d", &menuOption);
	}
	
	// Portal variables
	int passcode, adminPasscode=105016; // Admin portal
	
	int option; 		           // Seat reservation portal
	char name[MAX],
		flight_number1[MAX]={'M', 'I', 'A', '1', '0', '1', '0', '\0'},
		flight_number2[MAX]={'B', 'N', 'A', '1', '0', '5', '0', '\0'},
		flight_number3[MAX]={'L', 'A', 'S', '1', '0', '5', '0', '\0'};

	// Access to portals 1, 2, 3
	switch (menuOption)
	{
		case 1: // Admin login 
			printf("\nEnter the login passcode to log in as admin: ");
			scanf("%d", &passcode);
			while (loginMatch(passcode, adminPasscode) !=1)
			{
				scanf("%d", &passcode);
			}

			// Print flight maps and total revenue
			printf("Successful log-in.\n");
			printf("Printing flight map from Columbia to Miami:\n");
			printFlightMap(flight1);
			printf("Printing flight map from Columbia to Nashville:\n");
			printFlightMap(flight2);
			printf("Printing flight map from Columbia to Las Vegas:\n");
			printFlightMap(flight3);
			printf("Total earnings from all flights: $%d.", getTotalRevenue(flight1, flight2, flight3));
			printf("\nYou are logged out now.\n");
		break;

		case 2: // Seat reservation	
			flightMenu();
			scanf("%d", &option);
			while(option<1||option>3)
			{
				printf("Error: invalid option. Try again: ");
				scanf("%d", &option);
			}

			printf("Enter your name: ");
			scanf("%s", name);

			switch (option)
			{
				case 1: seatReservation(flight1);
					printMessage(name, flight_number1);
				break;

				case 2: seatReservation(flight2);
					printMessage(name, flight_number2);
				break;

				case 3: seatReservation(flight3);
					printMessage(name, flight_number3);
				break;
			}

		break;

		case 3: // Exit
			printf("\nGoodbye.\n");
		break;
	}

	return 0;
}

// Initialize seats
void initialSeats(char flight[ROW][COL], int count)
{
	// Set all seats to 'O's
	int a, b;
	for (a=0; a<ROW; a++)
	{
		for (b=0; b<COL; b++)
		{
			flight[a][b]='O';
		}
	}

	// Set rand seats to 'X's
	int c;
	for (c=0; c<count; c++)
	{
		int xRow=rand()%ROW, xCol=rand()%COL;
		if (flight[xRow][xCol]=='X')
		{
			c--;
		}
		flight[xRow][xCol]='X';
	}
}

// Display menu
void displayMenu(void)
{
	printf("1) Admin log-in\n2) Reserve a seat\n3) Exit\n");
}

// Print flight map
void printFlightMap(char flight[ROW][COL])
{
	int a, b;
	for (a=0; a<ROW; a++)
	{
		for (b=0; b<COL; b++)
		{
			printf("%3C", flight[a][b]);
		}
		printf("\n");
	}
	printf("\n");
}

// Login match
int loginMatch(int passcode, int adminPasscode)
{
	if (passcode==adminPasscode)
	{
		return 1;
	}
	else
	{
		printf("Error: invalid passcode. Try again: ");
		return 0;
	}
}

// Get total revenue
int getTotalRevenue(char flight1[ROW][COL], char flight2[ROW][COL],char flight3[ROW][COL])
{
	int a, b, subtotal1=0, subtotal2=0, subtotal3=0, total=0;
	for (a=0; a<ROW; a++)
	{
		for (b=0; b<COL; b++)
		{
			if (flight1[a][b]=='X')
			{
				subtotal1 += costMatrix[a][b];
			}
		}
	}

	for (a=0; a<ROW; a++)
	{
		for (b=0; b<COL; b++)
		{
			if (flight2[a][b]=='X')
			{
				subtotal2 += costMatrix[a][b];
			}
		}
	}
	
	for (a=0; a<ROW; a++)
	{
		for (b=0; b<COL; b++)
		{
			if (flight3[a][b]=='X')
			{
				subtotal3 += costMatrix[a][b];
			}
		}
	}
	
	return total = subtotal1 + subtotal2 + subtotal3;
}

// Flight menu
void flightMenu(void)
{
	printf("\nFlight 1: COU -> MIA\nFlight 2: COU -> BNA\nFlight 3: COU -> LAS\nEnter a flight option: ");
}

// Seat reservation
void seatReservation(char flight[ROW][COL])
{
	int status=-1;
	printf("\nHere are available seats:\n");
	printFlightMap(flight);
	
	// Outer loop to check availability of seat
	do
	{
		// User enters seat index with error check
		int row, col;
		printf("Enter your desired Row Number: ");
		scanf("%d", &row);
		while (row<1||row>6)
		{
			printf("Error: enter a value between 1 and 6: ");
			scanf("%d", &row);
		}
		row = row-1;
	
		printf("Enter your desired Column Number: ");
		scanf("%d", &col);
		while (col<1||col>4)
		{
			printf("Error: enter a value between 1 and 4: ");
			scanf("%d", &col);
		}
		col = col-1;
	
		// Check availability of seat
		if (flight[row][col]=='X')
		{
			printf("Sorry, that seat is unavailable.\n");
			status=0;
		}
		else
		{
			flight[row][col]='X';
			status=1;
		}
	}
	while (status!=1);

	// Reprint map with selected seat's 'O' replaced to 'X' and print success message
	printf("\nSuccess! Your seat has been reserved.\n");
	printFlightMap(flight);
}

// Print flight booked message
void printMessage(char name[], char flight_number[])
{
	printf("Congratulations %s, you are booked for flight %s. Enjoy your travel!\n", name, flight_number);
}






































































































