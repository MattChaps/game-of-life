#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>

#include"gol.h"

int main (int argc, char *argv[]) 
{
	char output_filename[300];
	char input_filename[300];
	char g_len;
	char g_str[11];

	bool i_specified = false;
	bool o_specified = false; 
	bool s_specified = false; 
	bool t_specified = false;
	bool g_specified = false;
	
	int number_of_generations = 5;
	
	int (*rule)(struct universe *, int, int) = will_be_alive;  

	struct universe u;

	while ( *++argv && argc > 0)
	{
		if ( (*argv)[0] == '-' )
		{
			if ( strlen( *argv ) > 2 )
			{
				fprintf(stderr, "Error: invalid case **%s**\n", *argv);
				exit(1);
			}
			
			switch((*argv)[1])
			{
				default:
						fprintf(stderr, "Error: invalid case **-%c**\n", (*argv)[1]);
						exit(1);
						break;

				case 'i':
						if ( i_specified )
						{
							if ( strcmp( *(argv+1), input_filename ) )
							{
								fprintf(stderr, "Error: cannot take input **%s** because -i already specified to **%s**\n", *(argv+1), input_filename );
								exit(1);
							}
						}
						
						if ( !*++argv )
						{
							fprintf(stderr, "Error: no input file specified\n" );
							exit(1);
						}
						
						strcpy( input_filename, *argv );
						
						i_specified = true;

						break;

				case 'o':
						if ( o_specified )
						{
							if ( strcmp( *(argv+1), output_filename ) )
							{
								fprintf(stderr, "Error: cannot output to **%s** because -o already specified to **%s**\n", *(argv+1), output_filename );
								exit(1);
							}
						}

						if ( !*++argv )
						{
							fprintf(stderr, "Error: no output file specified\n" );
							exit(1);
						}
						
						strcpy( output_filename, *argv );
						
						o_specified = true;

						break;

				case 'g':
						if ( g_specified )
						{
							sprintf( g_str, "%d", number_of_generations );

							if ( strcmp( *(argv+1), g_str ) )
							{
								fprintf(stderr, "Error: cannot set number of new generations to **%s** because -g already specified to **%d**\n", *(argv+1), number_of_generations );
								exit(1);
							}
						}

						if ( !*++argv )
						{
							fprintf(stderr, "Error: no number of new generations specified\n" );
							exit(1);
						}

						 g_len = strlen ( *argv );
						
						for ( int i = 0; i <  g_len; i++ )
							if ( !isdigit( (*argv)[i] ) )
							{
								fprintf ( stderr, "Error: **%s** is not a numerical number of generations\n", *argv );
								exit(1);
							}

						number_of_generations = atoi( *argv );

						g_specified = true;
						
						break;

				case 's':
						s_specified = true;
						
						break;

				case 't':
						t_specified = true;
						
						break;
            }
		}
		else
		{
			fprintf( stderr, "Error: unkown command **%s**\n", *argv );
			exit(1);
		}
	}


/*
 * Command-line switch: -i input_filename
 * --------------------
 * to specify that the initial generation of the universe should be read from a file. If
 * this option is not specified, you should let the user type in the input
*/
	if ( i_specified )
		{
			FILE *infile = fopen( input_filename, "r" );
						
			read_in_file( infile, &u );
		}
	else
		read_in_file( stdin, &u );
	

/*
 * Command-line switch: -t
 * --------------------
 * use the torus topology for the rule. If this is not specified, use the rule for cells outside the
 * universe being permanently dead
*/
	if ( t_specified )
		rule = will_be_alive_torus;

	for ( int i = 0; i < number_of_generations; i++ )
		evolve( &u, rule );
	

/*
 * Command-line switch: -o output_filename
 * --------------------
 * specify a file into which the final generation should be output. If this option
 * is not specified, you should output the final generation on the screen
*/
	if ( o_specified )
	{
		FILE *outfile = fopen( output_filename, "w" );
		
		write_out_file( outfile, &u );
	}
	else
	{ 
		write_out_file(stdout, &u);
	}
	

/*
 * Command-line switch: -s
 * --------------------
 * s to print statistics after the final generation has been output
*/
	if ( s_specified ) 
		print_statistics( &u );
	


	return 0;
}

