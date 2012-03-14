/*
 * sttyl - lite version of GNU stty
 */

#include <termios.h>
#include <stdio.h>	// perror()
#include <stdlib.h>	// exit()
#include <string.h>	// strcmp()
#include <ctype.h>	// isdigit()
#include <stddef.h>	// NULL

#include "defs.h"

int check_speed( struct termios *ttyopts, char *av );
int check_special( struct termios *ttyopts, char *av, char *next);
int check_setting( struct termios *ttyopts, char *av );
void set_argvs( struct termios *ttyopts, int ac, char *av[] );
void print_all( struct termios *ttyopts );

static char *program_name;

int main( int ac, char **av )
{
	struct termios ttyopts;

	program_name = av[0];

	// Always attempt to get terminal parameters
	if ( tcgetattr(FD, &ttyopts) == -1 ) {
		perror("tcgetattr of stdin");
		exit(1);
	}

	if ( ac <= 1 ) {
		print_all(&ttyopts);
	}
	else {
		set_argvs(&ttyopts, ac, av);
	}

	exit(0);
}

/*
 * Print all of the available terminal device settings.
 */
void print_all( struct termios *ttyopts )
{
	print_speed(ttyopts);		// Print the speed
	print_special(ttyopts);		// Print all special chars
	print_setting(ttyopts);		// Print all settings
}

/*
 * Match each argument variable parameter against available settings.
 * If no match is found print an error message.
 */
void set_argvs( struct termios *ttyopts, int ac, char *av[] )
{
	int i;
	for (i = 1; i < ac; i++) {
		if ( check_speed(ttyopts, av[i]) == 0 ) {
			continue;
		}
		else if ( check_special(ttyopts, av[i], av[i+1]) == 0 ) {
			i++;
			continue;
		}
		else if ( check_setting(ttyopts, av[i]) == 0 ) {
			continue;
		}
		else {
			fprintf(stderr, "%s: invalid argument `%s'\n",
				program_name, av[i]);
		}
	}
}

/*
 * Check if the input character pointer is a valid speed argument. If
 * so, set the device speed with the index returned by is_speed and
 * return 0. If the input character pointer is invalid then return -1
 */
int check_speed( struct termios *ttyopts, char *av )
{
	int i = is_speed(av);
	if ( i != -1 ) {
		set_speed(ttyopts, i);
		return 0;
	}
	else {
		return -1;
	}
}

/*
 * Check if the input argument variable is a valid special character.
 * If so, set the appropriate device special character element and
 * return 0. If the input character pointer is invalid then return -1.
 */
int check_special( struct termios *ttyopts, char *av, char *next)
{
	int i = is_special(av);
	if ( i != -1) {
		if ( next == NULL ) {
			fprintf(stderr, "%s: missing argument to `erase'\n",
					program_name);
			exit(1);
		}
		if ( set_special(ttyopts, i, next) == -1 ) {
			fprintf(stderr, "%s: Invalid integer argument `%s'\n",
					program_name, next);
			exit(1);
		}
		return 0;
	}
	return -1;
}

/*
 * Check if the input argument variable is a valid device setting. If
 * so, turn the setting on or off depending on whether it is prefixed
 * with a dash. Return -1 if the input does not match, or 0 on success.
 */
int check_setting( struct termios *ttyopts, char *av )
{
	int on = 1;
	if (av[0] == '-') {
		on = 0;
		av++;
	}
	int i = is_setting(av);
	if ( i != -1 ) {
		set_setting(ttyopts, i, on);
		return 0;
	}
	return -1;
}

