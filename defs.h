/*
 * defs.h - function declarations, and file descriptor definitions
 */

#ifndef FD
#define FD 0	// Assume standard input for all calls to tcsetattr()
#endif

#ifndef DEFS_H
#define DEFS_H
/* speed.c */
int is_speed( char *av );
void set_speed( struct termios *ttyopts, int i );
void print_speed( struct termios *ttyopts );
/* special.c */
int is_special( char *av );
int set_special( struct termios *ttyopts, int i, char *c);
void print_special( struct termios *ttyopts );
/* setting.c */
int is_setting( char *av );
void set_setting( struct termios *ttyopts, int i, int on );
void print_setting( struct termios *ttyopts );
#endif

