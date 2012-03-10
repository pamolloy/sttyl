/*
 * sttyl - TODO
 */

#include <termios.h>
#include <stdio.h>	// perror()
#include <stdlib.h>	// exit()
#include <string.h>	// strcmp()
#include <ctype.h>	// isdigit()

#include "sttyl.h"	// FD
#include "character.c"
#include "special.c"
#include "speed.c"

int main( int ac, char **av )
{
	struct termios ttyopts;

	// Always attempt to get terminal parameters
	if ( tcgetattr(FD, &ttyopts) == -1 ) {
		perror("tcgetattr of stdin");
		exit(1);
	}

	parse_args(ac, av);

	exit(0);
}

void parse_args( int ac, char **av)
{
	if ( ac <= 1 ) {
		print_speed(&ttyopts);				// Print the speed
		print_special(&ttyopts);			// Print all special characters
		loop_setting(&ttyopts, NULL, 0);	// Print all settings
	}
	else {
		int i;
		for (i = 1; i < ac; i++) {
			if ( check_speed(av[i]) == 0 )
				continue;
			if ( check_special(av[i], av[i] + 1) == 0 ) {
				i++;
				continue;
			}
			if ( check_setting(av[i]) == 0 )
				continue;
			fprintf(stderr, "%s: invalid argument `%s'", program_name, av[i]);
		}
	}
}

//TODO The following functions have not been tested

int check_speed( char *av )
{
	if ( is_speed(av[i]) != -1 ) {
		set_speed(&ttyopts, av[i]);
		return 0;
	}
	return -1;
}

int check_special( char *av, char *next)
{
	int i = is_special(av[i]);
	if ( i != -1 ) {
		if ( set_special(&ttyopts, i, next) == -1 ) {
			fprintf(stderr, "%s: Invalid integer argument \
					`%s'\n", program_name, next);
			exit(1);
		}
		return 0;
	}
	return -1;
}

int check_setting( char *av )
{
	int i = is_setting(av[i]);
	if ( i != -1 ) {
		if (av[i][0] == '-')
			set_setting(&ttyopts, av + 1, 1);
		else
			set_setting(&ttyopts, av, 0);
	}
	return -1;
}

