/*
 * lib/speed.c - functions to print and set the baud speed
 *
 * DATA STRUCTURES
 *	A structure containing the constant, which represents the baud
 *	speed and a corresponding name.
 *		spd( constant, name)
 *
 *	An array of speed structures that each contain an available speed
 *		baud_spds[]
 *
 * FUNCTIONS
 *	Print the baud speed stored in the termios structure
 *		printspd( struct termios )
 *
 *	Set the baud speed based on a name assigned in the speed structure
 *		setspd( struct termios, speed )
 */

#include <termios.h>	// struct termios
#include <stddef.h>		// NULL
#include <string.h>		// strcmp()
#include <stdio.h>		// printf()

void printspd( struct termios *ttyopts );
int setspd ( struct termios *ttyopts, char *spd );

struct spd { speed_t constant; char *name; };

struct spd baud_spds[] = {
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

int set_speed ( struct termios *ttyopts, char *spd )
{
	int i;

	for ( i = 0; baud_spds[i].name; i++ ) {
		if ( strcmp(baud_spds[i].name, spd) == 0 ) {
			if ( cfsetospeed(ttyopts, baud_spds[i].constant) == -1 ) {
				perror("cfsetospeed");
				exit(1);
			}
			return 0;
		}
	}
	return -1;
}

void print_speed( struct termios *ttyopts )
{
	int cfospeed = cfgetospeed(ttyopts);
	
	int i;
	for ( i = 0; baud_spds[i].name; i++ ) {
		if ( cfospeed & baud_spds[i].constant ) {
			printf(baud_spds[i].name);
		}
	}
}

