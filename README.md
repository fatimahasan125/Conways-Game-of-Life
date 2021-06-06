# Conways-Game-of-Life
The game of life implemented in C 

Description of the Game of Life taken from Wikipedia: \
The Game of Life is a cellular automaton devised by the British mathematician John Horton Conway in 1970. It is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. One interacts with the Game of Life by creating an initial configuration and observing how it evolves. 

The universe of the Game of Life is an infinite, two-dimensional orthogonal grid of square cells, each of which is in one of two possible states, live or dead, (or populated and unpopulated, respectively). Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent. At each step in time, the following transitions occur:

1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.


These rules, which compare the behavior of the automaton to real life, can be condensed into the following:

1. Any live cell with two or three live neighbours survives.
2. Any dead cell with three live neighbours becomes a live cell.
3. All other live cells die in the next generation. Similarly, all other dead cells stay dead.

The initial pattern constitutes the seed of the system. The first generation is created by applying the above rules simultaneously to every cell in the seed, live or dead; births and deaths occur simultaneously, and the discrete moment at which this happens is sometimes called a tick. Each generation is a pure function of the preceding one. The rules continue to be applied repeatedly to create further generations.

Some information about the code:
The board has 24 columns and 79 rows. I have divided it based on the number of columns. I have created 24 threads, each of which performs the working on one column. The phase 1 of the barrier is called to ensure that the board is modified if and only if all the threads have finished their working. After modifying the board(i.e the next generation) the phase 2 of the barrier is called before the thread ends to ensure that all the threads finish together, and the entire board is modified before printing.

## How to run 

To compile the code, use the following command in your ubuntu terminal:

gcc gameoflife.c -pthread

To execute it, enter the following command

./a.out input.txt

Make sure the input file is in the current working directory.

An example is shown in the video below: 


https://user-images.githubusercontent.com/68266703/120940876-e80a0980-c738-11eb-8521-7b5d281e9719.mp4


