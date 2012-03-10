/*
 * lib/character.c - functions to print and set special characters
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

struct spl {
	tcflag_t constant;
	char *name;
	void (*printer)(struct spl *, struct termios *);
	int (*setter)(struct spl *, struct termios *, char *);
};

void printspl( struct spl *row, struct termios *ttyopts );
int setspl( struct spl *row, struct termios *ttyopts, char *opt);

struct spl splchars[] = {
	{ VINTR	, "intr"	, printspl	, setspl	},
	{ VERASE, "erase"	, printspl	, setspl	},
	{ VKILL	, "kill"	, printspl	, setspl	},
	{ 0		, NULL		, 0			, 0			}
};

int setspl( struct spl *row, struct termios *ttyopts, char *opt)
{
	ttyopts->c_cc[row->constant] = *opt;

	if (tcsetattr(FD, TCSANOW, ttyopts) == -1) {
		perror("tcsetattr");
		return -1;
	}
	return 0;
}

/*
 * TODO
 */
void printc( int c )
{
	//TODO Replace with iscntrl?
	if ( isprint(c) != 0 ) {
		putchar(c);
	}
	else {
		printf("^%c", c - 1 + 'A');
	}
}

void printspl( struct spl *row, struct termios *ttyopts )
{
	printf(row->name);
	printf(" = ");
	printc(ttyopts->c_cc[row->constant]);
}

