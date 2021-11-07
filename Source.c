// ID: < 207023813>
// full name: < Shirley Alus >

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_ROWS 25
#define MAX_COLS 75
#define INCREASE_IN_STEP 6
#define SYMBOL '@'

/*************** Structs ***************/

typedef struct snakeNode
{
	int x;
	int y;
	struct snakeNode* next;
}SNAKENODE;

/*********** Declarations ************/

/// terminal escape codes/ANSI escape codes (or ANSI escape sequences)
///"\x1B" is the escape character that tells your terminal that what comes next is not meant to be printed on the screen, 
/// but rather a command to the terminal (or most likely terminal emulatoion).

/// Turn escape codes ON.
void init(void);

/// Moves the cursor to position (x,y) on screen.
/// Parameters:
///     x: the row of the posiiton.
///     y: the column of the posiiton.
void gotoxy(int x, int y);

/// Delays the execution of the program.
/// Parameters:
///    secs: the length of the delay in seconds. 
void sleep(float secs);

/// delay the sleep's timer in every 5th step.
void MoveFaster(float* time);

///prints the boarders of the game (25x75)
void PrintBoard();

/// processing user's input 
/// move the snake to the requested location
SNAKENODE* MoveSnake(SNAKENODE* head, char direction, int steps);

///creates a new new node 
SNAKENODE* createNewNode(int x, int y, SNAKENODE* next);

/// moves a new node to the head of the snake
SNAKENODE* Node2Head(SNAKENODE* head, SNAKENODE* newHead);

///prints the new head of the snake
void printHead(SNAKENODE* head);

/// free the tail of the snake
void freeTail(SNAKENODE* head);

/// checks if the snake met the board 
/// checks if the head of the snake hit one of his bodypart
bool ContinueGame(SNAKENODE* head);

/// clear the board
/// write gameover
void gameOver(SNAKENODE* head);

///free all the snake node
void freeAll(SNAKENODE* head);

/************** Main ***************/

int main()
{
	init();
	PrintBoard();

	char direction = 'd';
	int steps = 1;
	float time = 1.00;
	bool continueGame = true;

	SNAKENODE* head = createNewNode(MAX_ROWS / 2, MAX_COLS / 2, NULL);

	while (ContinueGame(head))
	{
		printHead(head);
		sleep(time);
		head = MoveSnake(head, direction, steps);
		steps++;

		if (_kbhit())
			direction = _getch();

		if ((steps % INCREASE_IN_STEP == 0))
			MoveFaster(&time);

	}

	gameOver(head);
}


/********** Implementations **********/

void init(void) {
	system("cls");
}

void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

void sleep(float secs) {
	clock_t clocks_start = clock();
	while (clock() - clocks_start < secs * CLOCKS_PER_SEC);
}

void MoveFaster(float* time) {
	if (*time <= 0.1)
		*time = (float)0.1;

	else
		*time -= (float)0.01;
}

void PrintBoard()
{
	for (int y = 1, x = 2; y <= MAX_COLS || x < MAX_ROWS; y++, x++)
	{
		gotoxy(1, y);
		printf("#");
		gotoxy(MAX_ROWS, y);
		printf("#");

		if (x < MAX_ROWS + 1) {
			gotoxy(x, 1);
			printf("#");
			gotoxy(x, MAX_COLS);
			printf("#");
		}
	}
	printf("\n");
}

void printHead(SNAKENODE* head)
{
	gotoxy(head->x, head->y);
	printf("%c", SYMBOL);
}


SNAKENODE* createNewNode(int x, int y, SNAKENODE* next)
{
	SNAKENODE* node = (SNAKENODE*)malloc(sizeof(SNAKENODE));

	node->x = x;
	node->y = y;
	node->next = next;

	return node;
}

SNAKENODE* Node2Head(SNAKENODE* head, SNAKENODE* newHead)
{
	newHead->next = head;
	return newHead;
}

SNAKENODE* MoveSnake(SNAKENODE* head, char direction, int steps)
{
	switch (direction)
	{
	case 'w': ///up
		head = Node2Head(head, createNewNode((head->x) - 1, head->y, head));
		break;
	case 's': /// down
		head = Node2Head(head, createNewNode((head->x) + 1, head->y, head));
		break;
	case 'a': /// left
		head = Node2Head(head, createNewNode(head->x, (head->y) - 1, head));
		break;
	case 'd': /// right
		head = Node2Head(head, createNewNode(head->x, (head->y) + 1, head));
		break;
	}


	if (steps % INCREASE_IN_STEP != 0)
	{
		freeTail(head);
	}

	return head;
}

void freeTail(SNAKENODE* head)
{
	SNAKENODE* current = head->next;
	SNAKENODE* previous = head;

	while (current->next != NULL)
	{
		previous = current;
		current = current->next;
	}

	gotoxy(current->x, current->y);
	printf(" ");
	free(current);
	previous->next = NULL;

}

bool ContinueGame(SNAKENODE* head)
{
	SNAKENODE* current = head;

	if (head->y == 1 || head->y == MAX_COLS || head->x == 1 || head->x == MAX_ROWS) /// checks if the head of the snake hit the borders of the board
		return false;

	while (current->next != NULL) /// checks if the head of the snake hit his body
	{
		if ((head->x == current->next->x) && (head->y == current->next->y))
		{
			return false;
		}

		current = current->next;
	}

	return true;

}


void gameOver(SNAKENODE* head)
{
	SNAKENODE* current = head->next;
	while (current != NULL)
	{
		gotoxy(current->x, current->y);
		printf(" ");
		current = current->next;
	}/// clear the board

	gotoxy(MAX_ROWS / 2, MAX_COLS / 3 + 6);
	printf("GAME OVER!!!");
	gotoxy(MAX_ROWS + 1, MAX_COLS + 1);
	printf("\n");

	freeAll(head);
}

void freeAll(SNAKENODE* head)
{
	SNAKENODE* next;

	while (head != NULL)
	{
		next = head->next;
		free(head);
		head = next;
	}
}



