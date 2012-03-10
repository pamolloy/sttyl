/*
 * TODO
 */

#include "sttyl.h"	// FD
#include <termios.h>
#include <stdio.h>	// perror()
#include <stdlib.h>	// exit()
#include <string.h>	// strcmp()

int main( int ac, char **av )
{
	struct termios ttyopts;

	// Always attempt to get terminal parameters
	if ( tcgetattr(FD, &ttyopts) == -1 ) {
		perror("tcgetattr of stdin");
		exit(1);
	}

	if ( ac <= 1 ) {
		loop_opts(&ttyopts, NULL, 0);	// Print all settings
	}
	else {		// Parse for setting, character, or speed
		int i;
		for (i = 1; i < ac; i++) {
			if (av[i][0] == '-')
				loop_opts(&ttyopts, av[i] + 1, 1);
			else
				loop_opts(&ttyopts, av[i], 0);
		}
	}

	exit(0);
}

void loop_opts( struct termios *ttyopts, char *opt, int on )
{
	int i;

	for ( i = 0; devopts[i].name; i++ ) {
		if ( opt == NULL ) {					// Print all options
			devopts[i].printer(&devopts[i], ttyopts);
		}
		else if ( opt == devopts[i].name) {		// Set one option
			devopts[i].setter(&devopts[i], ttyopts, on);
		}
	}
}

