/*
 * lib/special.c - functions to print and set special characters
 *
 * DATA STRUCTURES
 *
 *  special( constant, name, printer, setter )
 *    A structure containing the constant that defines the element in
 *    the termios special character array and a corresponding name
 *
 *  special_table[]
 *    An array of special character structures containing all of the
 *    implemented constants
 */

#include <termios.h>	// struct termios
#include <ctype.h>		// isprint()
#include <stddef.h>		// NULL
#include <stdio.h>		// printf(), putchar()
#include <string.h>		// strcmp()
#include <stdlib.h>		// exit()

#include "defs.h"

#define DEL 127	// ASCII decimal value for the delete character

struct special { tcflag_t constant; char *name; };

struct special special_table[] = {
	{ VINTR	, "intr"	},
	{ VERASE, "erase"	},
	{ VKILL	, "kill"	},
	{ 0		, NULL		}
};

/*
 * Compare the input character array to those in the special_table
 * array. If a match is found then return the index of the
 * corresponding special character structure. If a match is not found
 * then return -1.
 */
int is_special( char *av )
{
	int i;
	for (i = 0; special_table[i].name; i++) {
		if ( strcmp(special_table[i].name, av) == 0 ) {
			return i;
		}
	}
	return -1;
}

/*
 * Set the input character to the element in the input termios special
 * character array that corresponds to the input index of special
 * table. If successful return 0. If the input character pointer does
 * not point to a alphabetic character then return -1.
 */
int set_special( struct termios *ttyopts, int i, char *c)
{
	if ( isascii(*c) == 0 ) {
		return -1;
	}

	ttyopts->c_cc[special_table[i].constant] = *c;

	if (tcsetattr(FD, TCSANOW, ttyopts) == -1) {
		perror("tcsetattr");
		exit(1);
	}
	return 0;
}

/*
 * If printable, print the input ascii character, otherwise if it is a
 * control character calculate the ascii value and print the character
 * with a caret. Otherwise, if the character is a delete character
 * print the corresponding character representation.
 */
void print_character( int c )
{
	if ( isprint(c) != 0 ) {
		putchar(c);
	}
	else if ( c == DEL ) {
		printf("^?");
	}
	else if ( iscntrl(c) != 0 ) {
		printf("^%c", c - 1 + 'A');
	}
}

/*
 * Print all special characters stored in the input termios structure
 * that are listed in the special table and their corresponding names
 */
void print_special( struct termios *ttyopts )
{
	int i;
	
	for (i = 0; special_table[i].name; i++) {
		printf(special_table[i].name);
		printf(" = ");
		print_character(ttyopts->c_cc[special_table[i].constant]);
		printf("; ");
	}
	printf("\n");
}

