#include <stdio.h>

#define DIM 9	// table length
#define SUB 3	// sub square length

/* adding a macro for for loops iterating columns or rows */
#define FOR(var) \
	for (int var = 0; var < DIM; ++var)

char puzzle[DIM][DIM] = {
	{5, 3, 0, 0, 7, 0, 0, 0, 0},
	{6, 0, 0, 1, 9, 5, 0, 0, 0},
	{0, 9, 8, 0, 0, 0, 0, 6, 0},
	{8, 0, 0, 0, 6, 0, 0, 0, 3},
	{4, 0, 0, 8, 0, 3, 0, 0, 1},
	{7, 0, 0, 0, 2, 0, 0, 0, 6},
	{0, 6, 0, 0, 0, 0, 2, 8, 0},
	{0, 0, 0, 4, 1, 9, 0, 0, 5},
	{0, 0, 0, 0, 8, 0, 0, 7, 9}
};

// keep guessed cells to know after solving the puzzle
char guess[DIM][DIM];

void draw()
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

int find_free(int *x, int *y)
{
	FOR(i)
		FOR(j)
			if (! puzzle[i][j]) {
				*x = i;
				*y = j;
				guess[i][j] = 1;
				return 1;
			}
	return 0;
}

int is_valid(int n, int x, int y)
{
	FOR(i)
		if (puzzle[x][i] == n || puzzle[i][y] == n)
			return 0;

	int x_square = x / SUB * SUB;
	int y_square = y / SUB * SUB;
	for (int i = x_square; i < x_square + SUB; ++i) 
		for (int j = y_square; j < y_square + SUB; ++j)
			if (puzzle[i][j] == n)
				return 0;
	return 1;
}

int solve()
{
	int x, y;
	if (!find_free(&x, &y))
		return 1;

	FOR(i)
		if (is_valid(i, x, y)) {
			puzzle[x][y] = i;
			if (solve())
				return 1;
			puzzle[x][y] = 0;
		}
	return 0;
}

int main ()
{
	if (solve())
		draw();
	else
		printf("I can not solve it! wow....\n");
	return 0;
}
