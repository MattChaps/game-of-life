#include"gol.h"

/*
 * Function:  read_in_file
 * --------------------
 * Reads in the file from the file pointer infile and stores the universe in the structure
 * pointed to by u. Uses dynamic memory allocation to set aside memory for storing the
 * universe.
 */
void read_in_file(FILE *infile, struct universe *u)
{
	int i = 0;
	int max_columns = 513;
	int difference;
	
	bool columns_found = false; 

	char cell;

	(*u).grid = (char *)malloc( max_columns );

	(*u).new_generations = 0;
			
	if ((*u).grid == NULL)
	{
		fprintf( stderr, "Error: malloc returned NULL when allocating memory for grid\n" );
		exit(1);
	}
	
	if (infile == NULL)
	{
		fprintf( stderr, "Error: invalid infile pointer\n" );
		exit(1);
	}

	while( (cell = fgetc( infile )) != EOF )
	{
		/* Ignore carriage return in Windows newline character */
		if ( cell == '\r') continue;

		(*u).grid[i] = cell;

		if ( cell == '\n' )
		{
				if ( !columns_found )
				{
					(*u).columns = i;
					
					columns_found = true;

					if ( (*u).columns > 512 )
					{
						fprintf( stderr, "Error: Number of columns **%d** exceeds maximum (512)", (*u).columns );
						exit(1);
					}
				}

				difference = i - ((*u).columns + (*u).rows * ((*u).columns + 1));

				if ( difference != 0 )
				{
					fprintf( stderr, "Error: unequal line length **%d**, expected %d\n", ((*u).columns + difference), (*u).columns );
					exit(1);
				}

				(*u).rows += 1;
				
				(*u).grid = (char *)realloc( (*u).grid, ((*u).rows + 1) * max_columns );
		}
		else if ( cell == '*')
			{
				(*u).cells_alive_cur += 1;
				(*u).cells_alive_total += 1;
			}
		else if ( cell != '.')
			{
				fprintf(stderr, "Error: invalid character **%c** in input grid\n", cell);
				exit(1);
			}
		
		i++;
	}

	(*u).cells = (*u).columns * (*u).rows;
    (*u).chars = i;

	if ( (*u).cells == 0 && (*u).rows != 0 )
	{
		fprintf( stderr, "Error: no cells in input grid\n");
		exit(1);
	}

	if ( (*u).grid[i-1] != '\n')
	{
		fprintf(stderr, "Error: no terminating newline character on last row in input\n");
		exit(1);
	}
}

/*
 * Function:  write_out_file
 * --------------------
 * writes the content of the universe pointed to by u into the file from the file pointer
 * outfile.
 */
void write_out_file(FILE *outfile, struct universe *u)
{
	/* Check for valid outfile pointer */
	if (outfile == NULL)
	{
		fprintf( stderr, "Error: cannot open file\n" );
		exit(1);
	}
	
	int i = 0;

	while ( i < (*u).chars )
	{
		fprintf( outfile, "%c", (*u).grid[i] );
		i++;
	}
}

/*
 * Function:  is_alive
 * --------------------
 * returns 1 if the cell in that column and row is alive and 0 otherwise
 */
int is_alive(struct universe *u, int column, int row)
{
	int index;

	bool valid_column = false;
	bool valid_row = false;
	
	if ( column >= 0 && column <= (*u).columns - 1 ) 
		valid_column = true;
	
	if ( row >= 0 && row <= (*u).rows - 1) 
		valid_row = true;

	if ( valid_column && valid_row ) 
	{
		index = column + row * ((*u).columns + 1);
		
		if ( (*u).grid[index] == '*' ) 
			return 1;
	}
	return 0;
}

/*
 * Function:  will_be_alive 
 * --------------------
 * returns 1 if the cell in that column and row will be alive in the next generation and
 * 0 otherwise, assuming that cells outside the universe are always dead
 */
int will_be_alive(struct universe *u, int column, int row)
{
	int column_array[3] = { column - 1, column, column + 1 };
	int row_array[3] = { row - 1, row, row + 1};
	int cell_alive; 
	int alive_neighbours = 0;

	for ( int i = 0; i < 3; i++ )
		{ 
			for ( int j = 0; j < 3; j++ )
			{
				if ( i == 1 && j == 1 )
					{
						cell_alive = is_alive( u, column_array[j], row_array[i] );
						
						continue;
					}

				alive_neighbours += is_alive( u, column_array[j], row_array[i] );
			}
		}

	if ( cell_alive  && ( alive_neighbours == 2 || alive_neighbours == 3 ) )
	{
		return 1;
	}
	else if ( !cell_alive && ( alive_neighbours == 3 ) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * Function:  will_be_alive_torus 
 * --------------------
 * returns 1 if the cell in that column and row will be alive in the next generation
 * and 0 otherwise, assuming a torus topology
*/
int will_be_alive_torus(struct universe *u,  int column, int row)
{	
	int column_array[3] = { (column - 1 + (*u).columns) % (*u).columns, 
						column, 
						(column + 1) % (*u).columns };
	
	int row_array[3] = { (row - 1 + (*u).rows) % (*u).rows, 
						row, 
						(row + 1) % (*u).rows };
	
	int cell_alive; 
	int alive_neighbours = 0;

	for ( int i = 0; i < 3; i++ )
		{ 
			for ( int j = 0; j < 3; j++ )
			{
				if ( i == 1 && j == 1 )
					{
						cell_alive = is_alive( u, column_array[j], row_array[i] );
						
						continue;
					}

				alive_neighbours += is_alive( u, column_array[j], row_array[i] );
			}
		}

	if ( cell_alive  && ( alive_neighbours == 2 || alive_neighbours == 3 ) )
	{
		return 1;
	}
	else if ( !cell_alive && ( alive_neighbours == 3 ) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * Function:  evolve
 * --------------------
 * changes the universe from the current generation to the next generation. It uses the function pointed to by the function 
 * pointer rule to decide whether a cell will be alive or dead, e.g.
 * will_be_alive or will_be_alive_torus
*/
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row))
{
	int cell_alive;
	int index;
	int new_grid[(*u).chars];

	(*u).cells_alive_cur = 0;

	for ( int i = 0; i < (*u).rows; i++ )
		{ 
			for ( int j = 0; j < (*u).columns + 1; j++ )
				{ 
					index = j + i * ((*u).columns + 1);
                    
					if ( (*u).grid[index] == '\n' ) 
					{
						new_grid[index] = '\n';
						continue;
					}
					
					cell_alive = rule( u, j, i );

					if ( cell_alive ) 
					{
						new_grid[index] = '*';
						(*u).cells_alive_cur += 1;
						(*u).cells_alive_total += 1;
					}
					else 
						new_grid[index] = '.';
				}
		}

	for ( int i = 0; i < (*u).chars; i++ )
		(*u).grid[i] = new_grid[i];
		
	(*u).new_generations += 1;
}

/*
 * Function:  print_statistics
 * --------------------
 * should calculate the percentage of cells that are alive in the current generation
 * and the average percentage that have been alive in all of the generations so far (including the original
 * generation). 
*/
void print_statistics(struct universe *u)
{
	float percen_cells_alive_cur = 100 * (*u).cells_alive_cur / (*u).cells;
	float percen_cells_alive_total = 100 * (*u).cells_alive_total / (*u).cells / ((*u).new_generations + 1) ;
	
	printf( "%.3f%% of cells currently alive\n", percen_cells_alive_cur );
	printf( "%.3f%% of cells alive on average\n", percen_cells_alive_total );
}

