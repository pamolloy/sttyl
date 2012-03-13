/*
 * lib/speed.c - functions to check, print, and set the baud speed
 *
 * DATA STRUCTURES
 *
 *  speed( constant, name)
 *    A structure containing the constant, which represents the baud
 *    speed, and a corresponding name
 *
 *  baud[]
 *    An array of speed structures that each contain a possible baud
 *    speed of the device
 */

#include <termios.h>	// struct termios
#include <stddef.h>		// NULL
#include <string.h>		// strcmp()
#include <stdio.h>		// printf()
#include <ctype.h>		// isdigit()
#include <stdlib.h>		// exit()

#include "defs.h"

struct speed { speed_t constant; char *name; };

struct speed baud[] = {
	{ B0		, "0"		},
	{ B50		, "50"		},
	{ B75		, "75"		},
	{ B110		, "110"		},
	{ B134		, "134"		},
	{ B150		, "150"		},
	{ B200		, "200"		},
	{ B300		, "300"		},
	{ B600		, "600"		},
	{ B1200		, "1200"	},
	{ B1800		, "1800"	},
	{ B2400		, "2400"	},
	{ B4800		, "4800"	},
	{ B9600		, "9600"	},
	{ B19200	, "19200"	},
	{ B38400	, "38400"	},
	{ B57600	, "57600"	},
	{ B115200	, "115200"	},
	{ B230400	, "230400"	},
	{ 0			, NULL		}
};

/*
 * Compare the input character array to those in the baud array. If a
 * match is found then return the index of the corresponding speed
 * structure. If a match is not found then return -1.
 */
int is_speed( char *av )
{
	int i;
	for ( i = 0; baud[i].name; i++ ) {
		if ( strcmp(baud[i].name, av) == 0 ) {
			return i;
		}
	}
	return -1;
}

/*
 * Set the baud speed in a termios structure to a constant listed in
 * the baud array, which is found using the index argument
 */
void set_speed( struct termios *ttyopts, int i )
{
	if ( cfsetospeed(ttyopts, baud[i].constant) == -1 ) {
		perror("cfsetospeed");
		exit(1);
	}
	if (tcsetattr(FD, TCSANOW, ttyopts) == -1) {
		perror("tcsetattr");
		exit(1);
	}
}

/*
 * Print a character array corresponding to the baud speed stored in
 * the termios structure.
 */
void print_speed( struct termios *ttyopts )
{
	int cfospeed = cfgetospeed(ttyopts);
	
	int i;
	for ( i = 0; baud[i].name; i++ ) {
		if ( cfospeed == baud[i].constant ) {
			printf("speed %s baud;\n", baud[i].name);
		}
	}
}

