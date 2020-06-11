# Conway’s Game of Life

Conway’s Game of Life is a cellular automaton invented by John Conway in 1970. The game world consists
of an infinite two-dimensional grid of cells, each of which is either “dead” or “alive” in each “generation.”
The game comes with a “rule” that explains how to calculate the state of a cell in the next generation.

Conway’s rule is as follows:

1. If a cell is alive in the current generation and exactly two or three of its neighbours are alive, then it
will be alive in the next generation.

2. If a cell is dead in the current generation and exactly three of its neighbours are alive, then it will be
alive in the next generation.

3. Otherwise, the cell will be dead in the next generation.

In this coursework, you will implement the Game of Life in C on a finite rectangular grid (the “universe”).

The file format for valid input and output files: each line represents one row of the grid and each character
represents one cell on that row; `‘*’` represents a live cell and `‘.’` represents a dead cell.

## A: Dynamically-linked library

Create a file `gol.c` that implements the functions:

1.  `read_in_file()` reads in the file from the file pointer `infile`
    and, using dynamic memory allocation, stores the universe in the structure pointed to by `u`.

2.  `write_out_file()` writes the content of the universe pointed to by
    `u` into the file from the file pointer `outfile`.

3.  `is_alive()` returns `1` if the cell in that column and row is alive
    and `0` otherwise.

4.  `will_be_alive()` returns `1` if the cell in that column and row
    will be alive in the next generation and `0` otherwise, assuming
    that cells outside the universe are always dead.

5.  `will_be_alive_torus()` returns `1` if the cell in that column and
    row will be alive in the next generation and `0` otherwise, assuming
    a torus topology.

6.  `evolve()` changes the universe from the current generation to the
    next generation. It uses the function pointed to by the function
    pointer `rule` to decide whether a cell will be alive or dead, e.g.
    `will_be_alive` or `will_be_alive_torus`.

7.  `print_statistics()` should calculate the percentage of cells that
    are alive in the current generation and the average percentage that
    have been alive in all of the generations so far (including the
    original generation).

## B: Command-line program

Provide a command-line program `gameoflife` to use your library. Support
the following command-line switches:

-   `-i input_filename` to specify that the initial generation of the
  universe should be read from a file. If this option is not
  specified, you should let the user type in the input.

-   `-o output_filename` to specify a file into which the final
  generation should be output. If this option is not specified, you
  should output the final generation on the screen.

-   `-g number_of_generations` to specify the number of new generations
  for which the game should be run. (Set to 5 if this option is not
  given.)

-   `-s` to print statistics after the final generation has been output.


-   `-t` to use the torus topology for the rule. If this is not
  specified, use the rule for cells outside the universe being
  permanently dead.

## C: Build system


Provide a `Makefile` to compile your library and your command-line
program. In your Makefile, use the `-Wall -Wextra -pedantic -std=c11`
options each time you call `gcc`. Have rules for creating an object file from each
`.c` file, for creating your dynamically-linked library `libgol.so` and
for creating your command-line program `gameoflife`. Using the command
`make` should compile your program to a file called `gameoflife` and
your library `libgol.so`, both in the current folder. Running
`make clean` should delete all files produced during compilation.
