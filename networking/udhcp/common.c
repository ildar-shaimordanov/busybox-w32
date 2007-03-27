/* vi: set sw=4 ts=4: */
/* common.c
 *
 * Functions for debugging and logging as well as some other
 * simple helper functions.
 *
 * Russ Dill <Russ.Dill@asu.edu> 2001-2003
 * Rewritten by Vladimir Oleynik <dzo@simtreas.ru> (C) 2003
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include <syslog.h>

#include "common.h"


long uptime(void)
{
	struct sysinfo info;
	sysinfo(&info);
	return info.uptime;
}

#if ENABLE_FEATURE_PIDFILE
static const char *saved_pidfile;

static void pidfile_delete(void)
{
	if (saved_pidfile)
		remove_pidfile(saved_pidfile);
}
#endif

static void create_pidfile(const char *pidfile)
{
	if (!pidfile)
		return;

	if (!write_pidfile(pidfile)) {
		bb_perror_msg("cannot create pidfile %s", pidfile);
		return;
	}
#if ENABLE_FEATURE_PIDFILE
	/* lockf(pid_fd, F_LOCK, 0); */
	if (!saved_pidfile)
		atexit(pidfile_delete);
	saved_pidfile = pidfile;
#endif
}

void udhcp_make_pidfile(const char *pidfile)
{
	/* Make sure fd 0,1,2 are open */
	bb_sanitize_stdio();

	/* Equivalent of doing a fflush after every \n */
	setlinebuf(stdout);

	/* Create pidfile */
	create_pidfile(pidfile);

	bb_info_msg("%s (v%s) started", applet_name, BB_VER);
}
