#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define FORMAT_BUFF 13 // max int possible charachters + 3, could be 5 but warnings should have been silenced

/* adding a macro for for loops iterating columns or rows */
#define FOR(var) \
	for (int var = 0; var < DIM; ++var)

static int DIM;	// table length
static int SUB;	// sub square length

/* print the board including the division */
void print_board(int **puzzle, bool **guess)
{
	// get max cell width to draw table
	int cell_width = 0;
	for (int tmp = DIM; tmp > 0; tmp /= 10)
		++cell_width;
	// making number width dynamic to be like "%(char width)d"
	char strformat[FORMAT_BUFF];
	sprintf(strformat, "%%%dd", cell_width);
	FOR (i) {
		/* assuming the valid cell values are one digit, one for cell value
		 * one for space after it and we need extra one for the first space
		 * so it will be 2 * DIM + 1 */
		if (! (i % SUB)) {
			for (int r = 0; r < DIM * (cell_width + 1) + 1; ++r)
				putchar('-');
			putchar('\n');
		}
		// putting vertical dividers
		FOR(j) {
		putchar(!(j % SUB) ? '|' : ' ');
			// set output color to print guessed cells in different color
			if (guess[i][j])
				printf("\033[32m");
			printf(strformat, puzzle[i][j]);
			printf("\033[0m");
		}
		puts("|");
	}
	for (int r = 0; r < DIM * (cell_width + 1) + 1; ++r)
		putchar('-');
	putchar('\n');
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

/* defining a type for storing each blank cell cordinates and valid choices */
typedef struct {
	int x;
	int y;
	int options;	// options available
} Cell;

/* compare blank cells from least to most valid choices */
int cell_cmp(const void *a, const void *b) {
    Cell *ca = (Cell *) a, *cb = (Cell *) b;
    if (ca -> options < cb -> options) return -1;
    if (ca -> options > cb -> options) return  1;
    return 0;
}

static Cell *order;
static int pos = 0, numblanks;

/* find the next blank cell to solve */
Cell *find_blanks(int **puzzle)
{
	Cell *blanks = malloc(DIM * DIM * sizeof(Cell));
	int count = 0;
	FOR(i) FOR(j) if (!puzzle[i][j]) {
		int opts = 0;
		for (int n = 1; n <= DIM; ++n)
			if (is_valid(n, i, j, puzzle)) ++opts;
		blanks[count++] = (Cell) {i, j, opts};
	}
	qsort(blanks, count, sizeof(Cell), cell_cmp);
	numblanks = count;
	return blanks;
}

/* solves the table using recursions */
bool solve(int **puzzle)
{
	if (pos >= numblanks)
		return true;

	int x = order[pos].x, y = order[pos].y;
	for(int i = 1; i <= DIM; ++i)
		if (is_valid(i, x, y, puzzle)) {
			puzzle[x][y] = i;
			++pos;
			if (solve(puzzle))
				return true;
			--pos;
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

	order = find_blanks(puzzle);
	if (!order) return 1;
	pos = 0;

	bool solved = solve(puzzle);
	if (solved)
		print_board(puzzle, guess);
	else
		printf("I can not solve it! wow...\n");

	// free allocated heap
	FOR(i) {
		free(puzzle[i]);
		free(guess[i]);
	}
	free(puzzle);
	free(guess);
	free(order);
	return 0;
}
