/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)defs.h	5.9 (Berkeley) %G%
 */

/*
 * Telnet server defines
 */
#include <sys/types.h>
#include <sys/param.h>

#ifndef	BSD
# define	BSD 43
#endif

#if	defined(CRAY) && !defined(LINEMODE)
# define SYSV_TERMIO
# define LINEMODE
# define KLUDGELINEMODE
# define NO_GETTYTAB
# define DIAGNOSTICS
# if !defined(UNICOS5)
#  define BFTPDAEMON
#  define HAS_IP_TOS
# endif
#endif /* CRAY */
#if defined(UNICOS5) && !defined(NO_SETSID)
# define NO_SETSID
#endif

#if defined(PRINTOPTIONS) && defined(DIAGNOSTICS)
#define TELOPTS
#define TELCMDS
#endif

#if	defined(SYSV_TERMIO) && !defined(USE_TERMIO)
# define	USE_TERMIO
#endif

#include <sys/socket.h>
#ifndef	CRAY
#include <sys/wait.h>
#endif	/* CRAY */
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#ifndef	FILIO_H
#include <sys/ioctl.h>
#else
#include <sys/filio.h>
#endif

#include <netinet/in.h>

#include <arpa/telnet.h>

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <syslog.h>
#ifndef	LOG_DAEMON
#define	LOG_DAEMON	0
#endif
#ifndef	LOG_ODELAY
#define	LOG_ODELAY	0
#endif
#include <ctype.h>
#ifndef NO_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#ifndef	USE_TERMIO
#include <sgtty.h>
#else
# ifdef	SYSV_TERMIO
# include <termio.h>
# else
# include <termios.h>
# endif
#endif
#if !defined(USE_TERMIO) || defined(NO_CC_T)
typedef unsigned char cc_t;
#endif

#ifndef _POSIX_VDISABLE
# ifdef VDISABLE
#  define _POSIX_VDISABLE VDISABLE
# else
#  define _POSIX_VDISABLE ((unsigned char)'\377')
# endif
#endif


#ifdef	CRAY
#include <sys/fcntl.h>
# ifdef	CRAY1
# include <sys/pty.h>
#  ifndef FD_ZERO
# include <sys/select.h>
#  endif /* FD_ZERO */
# endif	/* CRAY1 */

#include <memory.h>
#endif	/* CRAY */

#if	!defined(TIOCSCTTY) && defined(TCSETCTTY)
# define	TIOCSCTTY TCSETCTTY
#endif

#ifndef	FD_SET
#ifndef	HAVE_fd_set
typedef struct fd_set { int fds_bits[1]; } fd_set;
#endif

#define	FD_SET(n, p)	((p)->fds_bits[0] |= (1<<(n)))
#define	FD_CLR(n, p)	((p)->fds_bits[0] &= ~(1<<(n)))
#define	FD_ISSET(n, p)	((p)->fds_bits[0] & (1<<(n)))
#define FD_ZERO(p)	((p)->fds_bits[0] = 0)
#endif	/* FD_SET */

/*
 * I/O data buffers defines
 */
#define	NETSLOP	64
#ifdef CRAY
#undef BUFSIZ
#define BUFSIZ  2048
#endif

#define	NIACCUM(c)	{   *netip++ = c; \
			    ncc++; \
			}

/* clock manipulations */
#define	settimer(x)	(clocks.x = ++clocks.system)
#define	sequenceIs(x,y)	(clocks.x < clocks.y)

/*
 * Linemode support states, in decreasing order of importance
 */
#define REAL_LINEMODE	0x02
#define KLUDGE_LINEMODE	0x01
#define NO_LINEMODE	0x00

/*
 * Structures of information for each special character function.
 */
typedef struct {
	unsigned char	flag;		/* the flags for this function */
	cc_t		val;		/* the value of the special character */
} slcent, *Slcent;

typedef struct {
	slcent		defset;		/* the default settings */
	slcent		current;	/* the current settings */
	cc_t		*sptr;		/* a pointer to the char in */
					/* system data structures */
} slcfun, *Slcfun;

#ifdef DIAGNOSTICS
/*
 * Diagnostics capabilities
 */
#define	TD_REPORT	0x01	/* Report operations to client */
#define TD_EXERCISE	0x02	/* Exercise client's implementation */
#define TD_NETDATA	0x04	/* Display received data stream */
#define TD_PTYDATA	0x08	/* Display data passed to pty */
#define	TD_OPTIONS	0x10	/* Report just telnet options */
#endif /* DIAGNOSTICS */

/*
 * We keep track of each side of the option negotiation.
 */

#define	MY_STATE_WILL		0x01
#define	MY_WANT_STATE_WILL	0x02
#define	MY_STATE_DO		0x04
#define	MY_WANT_STATE_DO	0x08

/*
 * Macros to check the current state of things
 */

#define	my_state_is_do(opt)		(options[opt]&MY_STATE_DO)
#define	my_state_is_will(opt)		(options[opt]&MY_STATE_WILL)
#define my_want_state_is_do(opt)	(options[opt]&MY_WANT_STATE_DO)
#define my_want_state_is_will(opt)	(options[opt]&MY_WANT_STATE_WILL)

#define	my_state_is_dont(opt)		(!my_state_is_do(opt))
#define	my_state_is_wont(opt)		(!my_state_is_will(opt))
#define my_want_state_is_dont(opt)	(!my_want_state_is_do(opt))
#define my_want_state_is_wont(opt)	(!my_want_state_is_will(opt))

#define	set_my_state_do(opt)		(options[opt] |= MY_STATE_DO)
#define	set_my_state_will(opt)		(options[opt] |= MY_STATE_WILL)
#define	set_my_want_state_do(opt)	(options[opt] |= MY_WANT_STATE_DO)
#define	set_my_want_state_will(opt)	(options[opt] |= MY_WANT_STATE_WILL)

#define	set_my_state_dont(opt)		(options[opt] &= ~MY_STATE_DO)
#define	set_my_state_wont(opt)		(options[opt] &= ~MY_STATE_WILL)
#define	set_my_want_state_dont(opt)	(options[opt] &= ~MY_WANT_STATE_DO)
#define	set_my_want_state_wont(opt)	(options[opt] &= ~MY_WANT_STATE_WILL)

/*
 * Tricky code here.  What we want to know is if the MY_STATE_WILL
 * and MY_WANT_STATE_WILL bits have the same value.  Since the two
 * bits are adjacent, a little arithmatic will show that by adding
 * in the lower bit, the upper bit will be set if the two bits were
 * different, and clear if they were the same.
 */
#define my_will_wont_is_changing(opt) \
			((options[opt]+MY_STATE_WILL) & MY_WANT_STATE_WILL)

#define my_do_dont_is_changing(opt) \
			((options[opt]+MY_STATE_DO) & MY_WANT_STATE_DO)

/*
 * Make everything symetrical
 */

#define	HIS_STATE_WILL			MY_STATE_DO
#define	HIS_WANT_STATE_WILL		MY_WANT_STATE_DO
#define HIS_STATE_DO			MY_STATE_WILL
#define HIS_WANT_STATE_DO		MY_WANT_STATE_WILL

#define	his_state_is_do			my_state_is_will
#define	his_state_is_will		my_state_is_do
#define his_want_state_is_do		my_want_state_is_will
#define his_want_state_is_will		my_want_state_is_do

#define	his_state_is_dont		my_state_is_wont
#define	his_state_is_wont		my_state_is_dont
#define his_want_state_is_dont		my_want_state_is_wont
#define his_want_state_is_wont		my_want_state_is_dont

#define	set_his_state_do		set_my_state_will
#define	set_his_state_will		set_my_state_do
#define	set_his_want_state_do		set_my_want_state_will
#define	set_his_want_state_will		set_my_want_state_do

#define	set_his_state_dont		set_my_state_wont
#define	set_his_state_wont		set_my_state_dont
#define	set_his_want_state_dont		set_my_want_state_wont
#define	set_his_want_state_wont		set_my_want_state_dont

#define his_will_wont_is_changing	my_do_dont_is_changing
#define his_do_dont_is_changing		my_will_wont_is_changing
