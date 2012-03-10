/*
 * lib/setting.c - functions to print and set the input, output, and
 * local settings
 *
 * DATA STRUCTURES
 *	A structure containing the constant, which defines the setting, a
 *	corresponding name, and pointers to printer and setter functions
 *		opt( constant, name, printer, setter )
 *
 *	An array of "opt" structures that each contain an available device
 *	option
 *		devopts[]
 *
 * FUNCTIONS
 *	Print whether an option stored in the termios structure is on or
 *	off using the stty syntax
 *		p_in( row, ttyopts )	// Print input setting
 *		p_out( row, ttyopts )	// Print output setting
 *		p_local( row, ttyopts )	// Print local settings
 *
 *	Set the device option setting
 *		s_in( row, ttyopts, on )	// Set input setting
 *		s_out( row, ttyopts, on )	// Set output setting
 *		s_local( row, ttyopts, on )	// Set local setting
 */

#include "../sttyl.h"	// FD
#include <termios.h>	// struct termios
#include <stddef.h>		// NULL
#include <string.h>		// strcmp()
#include <stdio.h>		// printf()

struct opt {
	tcflag_t constant;
	char *name;
	void (*printer)(struct opt *, struct termios *);
	void (*setter)(struct opt *, struct termios *, int);
};

void s_in( struct opt *row, struct termios *ttyopts, int on );
void s_out( struct opt *row, struct termios *ttyopts, int on );
void s_local( struct opt *row, struct termios *ttyopts, int on );
void p_in( struct opt *row, struct termios *ttyopts );
void p_out( struct opt *row, struct termios *ttyopts );
void p_local( struct opt *row, struct termios *ttyopts );

struct opt devopts[] = {
	{ ICRNL		, "icrnl"	, p_in		, s_in		},
	{ ONLCR		, "onlcr"	, p_out		, s_out		},
	{ OLCUC		, "olcuc"	, p_out		, s_out		},
	{ ECHO		, "echo"	, p_local	, s_local	},
	{ ECHOE		, "echoe"	, p_local	, s_local	},
	{ ICANON	, "icanon"	, p_local	, s_local	},
	{ ISIG		, "isig"	, p_local	, s_local	},
	{ 0			, NULL		, 0			, 0			}
};

void loop_setting( struct termios *ttyopts, char *opt, int on )
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

/*
 * TODO
 */
int s_opt( struct opt *row, struct termios *ttyopts, tcflag_t *flag_p, int on )
{
	if ( on == 1 )
		*flag_p |= row->constant;
	else
		*flag_p &= ~(row->constant);

	if (tcsetattr(FD, TCSANOW, ttyopts) == -1) {
		perror("tcsetattr");
		return -1;
	}
	return 0;
}

void s_in( struct opt *row, struct termios *ttyopts, int on )
{
	s_opt(row, ttyopts, &ttyopts->c_iflag, on);
}

void s_out( struct opt *row, struct termios *ttyopts, int on )
{
	s_opt(row, ttyopts, &ttyopts->c_oflag, on);
}

void s_local( struct opt *row, struct termios *ttyopts, int on )
{
	s_opt(row, ttyopts, &ttyopts->c_lflag, on);
}

/*
 * TODO
 */
void p_opt ( struct opt *row, int flag)
{
	if ( flag & row->constant )
		printf(row->name);
	else
		printf("-%s", row->name);
}

void p_in( struct opt *row, struct termios *ttyopts )
{
	p_opt(row, ttyopts->c_iflag);
}

void p_out( struct opt *row, struct termios *ttyopts )
{
	p_opt(row, ttyopts->c_oflag);
}

void p_local( struct opt *row, struct termios *ttyopts )
{
	p_opt(row, ttyopts->c_lflag);
}

