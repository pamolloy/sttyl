struct option {
	tcflag_t constant;
	char *name;
	void(*printer)(struct option *, struct termios *);
	void(*setter)(struct option *, struct termios *, int);
};

struct option devopts[] = {
	VINTR	, "intr"	, &p_ctrl	, s_ctrl,
	VERASE	, "erase"	, p_ctrl	, s_ctrl,
	VKILL	, "kill"	, p_ctrl	, s_ctrl,
	ICRNL	, "icrnl"	, p_in		, s_in,
	ONLCR	, "onlcr"	, p_out		, s_out,
	OLCUC	, "olcuc"	, p_out		, s_out,
	ECHO	, "echo"	, p_local	, s_local,
	ECHOE	, "echoe"	, p_local	, s_local,
	ICANON	, "icanon"	, p_local	, s_local,
	ISIG	, "isig"	, p_local	, s_local,
	0		, NULL		, 0				, 0
};

/* SET */

void s_in( struct option *row, struct termios *ttyopts, int on)
{
	s_opt(row, ttyopts, &ttyopts->i_lflag, on)
}

void s_out( struct option *row, struct termios *ttyopts, int on)
{
	s_opt(row, ttyopts, &ttyopts->i_lflag, on)
}

void s_local( struct option *row, struct termios *ttyopts, int on)
{
	s_opt(row, ttyopts, &ttyopts->i_lflag, on)
}

void s_opt( struct option *row, struct termios *ttyopts, int *flag_p, int on )
{
	if ( on == 1 )
		*flag_p |= row.constant;
	else
		*flag_p &= ~(row.constant);

	if (tcsetattr(FD, TCSANOW, ttyopts) == -1) {
		perror("tcsetattr");
		return -1
	}
}

/* PRINT */

void p_ctrl( struct option *row, struct termios *ttyopts )
{
	printf(row.name);
	printf(" = ");
	p_char(ttyopts->cc_c[row.constant]);
}

