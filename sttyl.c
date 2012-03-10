/*
 * TODO
 */

#include "sttyl.h"	// FD
#include <termios.h>
#include <stdio.h>	// perror()
#include <stdlib.h>	// exit()
#include <string.h>	// strcmp()
#include <ctype.h>	// isdigit()

int main( int ac, char **av )
{
	struct termios ttyopts;

	// Always attempt to get terminal parameters
	if ( tcgetattr(FD, &ttyopts) == -1 ) {
		perror("tcgetattr of stdin");
		exit(1);
	}

	if ( ac <= 1 ) {
		print_speed(&ttyopts);				// Print the speed
		print_special(&ttyopts);			// Print all special characters
		loop_setting(&ttyopts, NULL, 0);	// Print all settings
	}
	else {		// Parse for input/output/local setting, special character, or speed
		int i;
		for (i = 1; i < ac; i++) {
			//TODO Create is_speed function
			if ( isdigit(av[i]) != 0 ) {	// If it's a digit then
				set_speed(&ttyopts, av[i]);	// Set the speed
				continue;
			}
			int special_i = is_special(av[i]);	// If argument is special character
			if ( special_i != -1 ) {
				if ( set_special(&ttyopts, special_i, av[i + 1]) == -1 ) {	// Set special character
					fprintf(stderr, "%s: Invalid integer argument \
							`%s'\n", program_name, av[i + 1]);
					exit(1);
				}
				i++;
				continue;
			}
			/*TODO Follow above pattern? (i.e. check if argument is a
			 * input/output/local setting, then set it?)
			 */
			if (av[i][0] == '-')
				loop_setting(&ttyopts, av[i] + 1, 1);
			else
				loop_opts(&ttyopts, av[i], 0);
		}
	}

	exit(0);
}

