#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* adding a macro for for loops iterating columns or rows */
#define FOR(var) \
	for (int var = 0; var < DIM; ++var)

int DIM;	// table length
int SUB;	// sub square length

/* print the table including the division */
void draw(int **puzzle, bool **guess)
{
	FOR (i) {
		/* assuming the valid cell values are one digit, one for cell value
		 * one for space after it and we need extra one for the first space
		 * so it will be 2 * DIM + 1 */
		if (! (i % SUB)) {
			for (int r = 0; r < DIM * 2 + 1; ++r)
				putchar('-');
			putchar('\n');
		}
		// putting vertical dividers
		FOR(j) {
		putchar(!(j % SUB) ? '|' : ' ');
			// set output color to print guessed cells in different color
			if (guess[i][j])
				printf("\033[32m");
			printf("%d\033[0m", puzzle[i][j]);
		}
		puts("|");
	}
	for (int r = 0; r < DIM * 2 + 1; ++r)
		putchar('-');
	putchar('\n');
}

/* find the next blank cell to solve */
bool find_free(int *x, int *y, int **puzzle)
{
	FOR(i)
		FOR(j)
			if (! puzzle[i][j]) {
				*x = i;
				*y = j;
				return true;
			}
	return false;
}

/* check if n can be placed in row x and col y */
bool is_valid(int n, int x, int y, int **puzzle)
{
	FOR(i)
		if (puzzle[x][i] == n || puzzle[i][y] == n)
			return false;

	int x_square = x / SUB * SUB;
	int y_square = y / SUB * SUB;
	for (int i = x_square; i < x_square + SUB; ++i) 
		for (int j = y_square; j < y_square + SUB; ++j)
			if (puzzle[i][j] == n)
				return false;
	return true;
}

/* solves the table using recursions */
bool solve(int **puzzle)
{
	int x, y;
	if (!find_free(&x, &y, puzzle))
		return true;

	for(int i = 1; i <= DIM; ++i)
		if (is_valid(i, x, y, puzzle)) {
			puzzle[x][y] = i;
			if (solve(puzzle))
				return true;
			puzzle[x][y] = 0;
		}
	return false;
}

int main (void)
{
	scanf("%d", &DIM);
	SUB = sqrt(DIM);
	if (SUB * SUB != DIM) return 1;	// DIM must be a perfect square

	/* allocate memory for getting input
	 * pointer to each row is allocated */
	int **puzzle = malloc(DIM * sizeof(int *));
	if (!puzzle) return 1;	//manipulate NULL pointer
	// now allocate rows and store its pointer in puzzle
	FOR(i) {
		puzzle[i] = malloc(DIM * sizeof(int));
		if (!puzzle[i]) return 1;
		FOR(j)
			scanf("%d", puzzle[i] + j);
	}

	// keep guessed cells to know after solving the puzzle
	bool **guess = malloc(DIM * sizeof(bool *));
	if (!guess) return 1;
	FOR(i) {
		guess[i] = malloc(DIM * sizeof(bool));
		if (!guess[i]) return 1;
	}
	FOR(i) FOR(j) guess[i][j] = puzzle[i][j] ? false : true;

	if (solve(puzzle))
		draw(puzzle, guess);
	else
		printf("I can not solve it! wow...\n");

	// free allocated heap
	FOR(i) {
		free(puzzle[i]);
		free(guess[i]);
	}
	free(puzzle);
	free(guess);
	return 0;
}
