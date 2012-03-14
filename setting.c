/*
 * lib/setting.c - functions to check, print, and set the input,
 * output, and local device settings
 *
 * DATA STRUCTURES
 *
 *  option( constant, name, printer, setter )
 *    A structure containing the constant, which defines the setting,
 *    a corresponding name, and pointers to printer and setter
 *    functions
 *
 *  setting_table[]
 *    An array of "opt" structures that each contain an available
 *    device option
 *
 * FUNCTIONS
 *
 *  p_in( row, ttyopts )
 *  p_out( row, ttyopts )
 *  p_local( row, ttyopts )
 *    Print whether an option stored in the termios structure is on or
 *    off using the stty syntax
 *
 *  s_in( row, ttyopts, on )
 *  s_out( row, ttyopts, on )
 *  s_local( row, ttyopts, on )
 *    Set the device option setting
 */

#include <termios.h>	// struct termios
#include <stddef.h>		// NULL
#include <string.h>		// strcmp()
#include <stdio.h>		// printf()
#include <stdlib.h>		// exit()

#include "defs.h"

struct option {
	tcflag_t constant;
	char *name;
	void (*printer)(struct option *, struct termios *);
	void (*setter)(struct option *, struct termios *, int);
};

void s_in( struct option *row, struct termios *ttyopts, int on );
void s_out( struct option *row, struct termios *ttyopts, int on );
void s_local( struct option *row, struct termios *ttyopts, int on );
void p_in( struct option *row, struct termios *ttyopts );
void p_out( struct option *row, struct termios *ttyopts );
void p_local( struct option *row, struct termios *ttyopts );

struct option setting_table[] = {
	{ ICRNL		, "icrnl"	, p_in		, s_in		},
	{ ONLCR		, "onlcr"	, p_out		, s_out		},
	{ OLCUC		, "olcuc"	, p_out		, s_out		},
	{ ECHO		, "echo"	, p_local	, s_local	},
	{ ECHOE		, "echoe"	, p_local	, s_local	},
	{ ICANON	, "icanon"	, p_local	, s_local	},
	{ ISIG		, "isig"	, p_local	, s_local	},
	{ 0			, NULL		, 0			, 0			}
};


/*
 * Compare the character array pointer parameter to the names listed
 * in setting_table. If a match is found then return the index of the
 * corresponding terminal device setting. Otherwise return -1.
 */
int is_setting( char *av )
{
	int i;
	for ( i = 0; setting_table[i].name; i++ ) {
		if ( strcmp(setting_table[i].name, av) == 0 ) {
			return i;
		}
	}
	return -1;
}

/*
 * Call the function corresponding to a table row identified by the
 * index parameter, int i.
 */
void set_setting( struct termios *ttyopts, int i, int on )
{
	setting_table[i].setter(&setting_table[i], ttyopts, on);
}

/*
 * Mask a terminal device setting either on (1) or off (0) within the
 * temporary termios structure, and then set the structure.
 */
void s_opt( struct option *row, 
			struct termios *ttyopts, 
			tcflag_t *flag_p, 
			int on )
{
	if ( on == 1 )
		*flag_p |= row->constant;
	else
		*flag_p &= ~(row->constant);

	if (tcsetattr(FD, TCSANOW, ttyopts) == -1) {
		perror("tcsetattr");
		exit(1);
	}
}

void s_in( struct option *row, struct termios *ttyopts, int on )
{
	s_opt(row, ttyopts, &ttyopts->c_iflag, on);
}

void s_out( struct option *row, struct termios *ttyopts, int on )
{
	s_opt(row, ttyopts, &ttyopts->c_oflag, on);
}

void s_local( struct option *row, struct termios *ttyopts, int on )
{
	s_opt(row, ttyopts, &ttyopts->c_lflag, on);
}

/*
 * Print each of the terminal device settings stored in the termios
 * structure, with the corresponding function in setting_table.
 */
void print_setting( struct termios *ttyopts )
{
	int i;
	for ( i = 0; setting_table[i].name; i++ ) {
		setting_table[i].printer(&setting_table[i], ttyopts);
		printf(" ");
	}
	printf("\n");
}

/*
 * Mask the constant found in a option structure with the flag parameter
 * and if true print its name, otherwise prepend a '-'.
 */
void p_opt ( struct option *row, int flag)
{
	if ( flag & row->constant )
		printf(row->name);
	else
		printf("-%s", row->name);
}

void p_in( struct option *row, struct termios *ttyopts )
{
	p_opt(row, ttyopts->c_iflag);
}

void p_out( struct option *row, struct termios *ttyopts )
{
	p_opt(row, ttyopts->c_oflag);
}

void p_local( struct option *row, struct termios *ttyopts )
{
	p_opt(row, ttyopts->c_lflag);
}

