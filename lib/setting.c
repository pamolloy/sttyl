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

#include "../sttyl.h"	// FD
#include <termios.h>	// struct termios
#include <stddef.h>		// NULL
#include <string.h>		// strcmp()
#include <stdio.h>		// printf()

void s_in( struct option *row, struct termios *ttyopts, int on );
void s_out( struct option *row, struct termios *ttyopts, int on );
void s_local( struct option *row, struct termios *ttyopts, int on );
void p_in( struct option *row, struct termios *ttyopts );
void p_out( struct option *row, struct termios *ttyopts );
void p_local( struct option *row, struct termios *ttyopts );

struct option {
	tcflag_t constant;
	char *name;
	void (*printer)(struct option *, struct termios *);
	void (*setter)(struct option *, struct termios *, int);
};

struct option devopts[] = {
	{ ICRNL		, "icrnl"	, p_in		, s_in		},
	{ ONLCR		, "onlcr"	, p_out		, s_out		},
	{ OLCUC		, "olcuc"	, p_out		, s_out		},
	{ ECHO		, "echo"	, p_local	, s_local	},
	{ ECHOE		, "echoe"	, p_local	, s_local	},
	{ ICANON	, "icanon"	, p_local	, s_local	},
	{ ISIG		, "isig"	, p_local	, s_local	},
	{ 0			, NULL		, 0			, 0			}
};

/* SET */

int s_opt( struct option *row, struct termios *ttyopts, tcflag_t *flag_p, int on )
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

/* PRINT */

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

