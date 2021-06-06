/*
 * The Game of Life
 *
 * a cell is born, if it has exactly three neighbours 
 * a cell dies of loneliness, if it has less than two neighbours 
 * a cell dies of overcrowding, if it has more than three neighbours 
 * a cell survives to the next generation, if it does not die of loneliness 
 * or overcrowding 
 *
 * In my version, a 2D array of ints is used.  A 1 cell is on, a 0 cell is off.
 * The game plays 100 rounds, printing to the screen each time.  'x' printed
 * means on, space means 0.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include "barrier.h"
#include <pthread.h>
#include <stdlib.h>

#define THREADS 24		//creating a thread for each row

/* dimensions of the screen */

#define BOARD_WIDTH	79
#define BOARD_HEIGHT	24

struct args{
	int** board;
	int colNumber;
};



struct Barrier barrier;
int newboard[BOARD_WIDTH][BOARD_HEIGHT];

/* set everthing to zero */

void initialize_board (int** board) {
	int	i, j;

	for (i=0; i<BOARD_WIDTH; i++) for (j=0; j<BOARD_HEIGHT; j++) 
		board[i][j] = 0;
}

/* add to a width index, wrapping around like a cylinder */

int xadd (int i, int a) {
	i += a;
	while (i < 0) i += BOARD_WIDTH;
	while (i >= BOARD_WIDTH) i -= BOARD_WIDTH;
	return i;
}

/* add to a height index, wrapping around */

int yadd (int i, int a) {
	i += a;
	while (i < 0) i += BOARD_HEIGHT;
	while (i >= BOARD_HEIGHT) i -= BOARD_HEIGHT;
	return i;
}

/* return the number of on cells adjacent to the i,j cell */

int adjacent_to (int** board, int i, int j) {
	int	k, l, count;

	count = 0;

	/* go around the cell */

	for (k=-1; k<=1; k++) for (l=-1; l<=1; l++)

		/* only count if at least one of k,l isn't zero */

		if (k || l)
			if (board[xadd(i,k)][yadd(j,l)]) count++;
	return count;
}

void* play (void* param) {

	struct args board_ = *(struct args*)param;
/*
	1.STASIS : If, for a given cell, the number of on neighbours is 
	exactly two, the cell maintains its status quo into the next 
	generation. If the cell is on, it stays on, if it is off, it stays off.

	2.GROWTH : If the number of on neighbours is exactly three, the cell 
	will be on in the next generation. This is regardless of the cell's 		current state.

	3.DEATH : If the number of on neighbours is 0, 1, 4-8, the cell will 
	be off in the next generation.
*/
	int	i, j, a;

	/* for each cell, apply the rules of Life */

	i = board_.colNumber;
	
	for (j=0; j<BOARD_WIDTH; j++) {
		a = adjacent_to (board_.board, j, i);
		if (a == 2) newboard[j][i] = board_.board[j][i];
		if (a == 3) newboard[j][i] = 1;
		if (a < 2) newboard[j][i] = 0;
		if (a > 3) newboard[j][i] = 0;
	}
	/* copy the new board back into the old board */

	phase1(&barrier);
	phase2(&barrier);

	for (j=0; j<BOARD_WIDTH; j++) {
		board_.board[j][i] = newboard[j][i];
	}


	pthread_exit(0);
}

/* print the life board */

void print (int** board) {
	int	i, j;

	/* for each row */

	for (j=0; j<BOARD_HEIGHT; j++) {

		/* print each column position... */

		for (i=0; i<BOARD_WIDTH; i++) {
			printf ("%c", board[i][j] ? 'x' : ' ');
		}

		/* followed by a carriage return */

		printf ("\n");
	}
}

/* read a file into the life board */

void read_file (int** board, char *name) {
	FILE	*f;
	int	i, j;
	char	s[100];

	f = fopen (name, "r");
	for (j=0; j<BOARD_HEIGHT; j++) {

		/* get a string */

		fgets (s, 100, f);

		/* copy the string to the life board */

		for (i=0; i<BOARD_WIDTH; i++) {
			board[i][j] = s[i] == 'x';
		}
	}
	fclose (f);
}

/* main program */

int main (int argc, char *argv[]) {
	int**	board, i, j;

	board = malloc(sizeof(int*)*BOARD_WIDTH);
	for(int i=0; i<BOARD_WIDTH; i++)
		board[i]=malloc(sizeof(int)*BOARD_HEIGHT);

	initialize_board (board);
	read_file (board, argv[1]);

	init(&barrier);
	
	/* play game of life 100 times */

	for (i=0; i<100; i++) {
		pthread_t id[THREADS];
		struct args arg[THREADS];
		for(j = 0; j<THREADS; j++){
			int k, l;
			
			arg[j].board = board;
			arg[j].colNumber = j;
			if(pthread_create(&id[j], NULL, &play, &arg[j])<0){
				printf("%s", "Error in creating thread\n");
				return -1;
			}

		}

		for(j=0; j<THREADS; j++){
			pthread_join(id[j], NULL);
		}
		print (board);
		sleep(1);

		/* clear the screen using VT100 escape codes */

		puts ("\033[H\033[J");
	}

	for(i=0; i<BOARD_WIDTH; i++)
		free(board[i]);
	free(board);

	return 0;
	
}
