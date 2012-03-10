/*
 * lib/special.c - functions to print and set special characters
 *
 * DATA STRUCTURES
 *	A structure containing the constant that defines the element in the
 *	cc_c array, a corresponding name, and pointers to printer and
 *	setter functions
 *		spl( constant, name, printer, setter )
 *
 *	An array of option structures that each contain a special character
 *		splchars[]
 *
 * FUNCTIONS
 *	Print the special character stored in the termios structure and its
 *	corresponding name
 *		printspl( struct special, struct termios )
 *
 *	If printable, print the character, otherwise calculate ascii value
 *	and print the character with a caret
 *		printc( character )
 *
 *	Set the character based on a name assigned in the special
 *	characters structure
 *		setspl( struct special, struct termios, character )
 */

#include "../sttyl.h"	// FD
#include <termios.h>	// struct termios
#include <ctype.h>		// isprint()
#include <stddef.h>		// NULL
#include <stdio.h>		// printf(), putchar()

struct special {
	tcflag_t constant;
	char *name;
	void (*printer)(struct special *, struct termios *);
	int (*setter)(struct special *, struct termios *, char *);
};

void printspl( struct special *row, struct termios *ttyopts );
int setspl( struct special *row, struct termios *ttyopts, char *opt);

struct special special_table[] = {
	{ VINTR	, "intr"	},
	{ VERASE, "erase"	},
	{ VKILL	, "kill"	},
	{ 0		, NULL		}
};

int is_special( char *av )
{
	int i;

	for (i = 0; special_table[i].name; i++) {
		if ( special_table[i].name ) {
			return i;
		}
	}
	return -1;
}

int set_special( struct termios *ttyopts, int i, char *c)
{
	if ( isalpha(c) == 0 ) {
		return -1;
	}

	ttyopts->c_cc[row->constant] = *c;

	if (tcsetattr(FD, TCSANOW, ttyopts) == -1) {
		perror("tcsetattr");
		exit(1);
	}
	return 0;
}

/*
 * TODO
 */
void print_character( int c )
{
	//TODO Replace with iscntrl?
	if ( isprint(c) != 0 ) {
		putchar(c);
	}
	else if ( iscntrl(c) != 0 ) {
		printf("^%c", c - 1 + 'A');
	}
	else {
		printf("^?");
	}
}

void print_special( struct termios *ttyopts )
{
	int i;
	
	for (i = 0; special_table[i].name; i++) {
		printf(special_table[i].name);
		printf(" = ");
		print_character(ttyopts->c_cc[special_table[i].constant]);
		printf("; ");
	}
}

