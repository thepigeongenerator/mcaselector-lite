/* Copyright (C)2025-2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if defined(__unix__)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "dat/mcx.h"
#include "err.h"
#include "io/getopt.h"
#include "types.h"
#include "util.h"

_Static_assert(-3 >> 5 == -1,
	"The platform does not compile "
	"right-shifting signed integers to an arithmetic shift!");

enum options {
	OPT_VERBOSE = 1,
	OPT_QUIET   = 2,
	OPT_CHECK   = 4,
	OPT_DEFRAG  = 8,
	OPT_REPAIR  = 16,

	/* Options that need write access to the file. */
	OPT_NEED_WRITE = OPT_DEFRAG | OPT_REPAIR,
};

const char        *argv0;
static const char *str_help =
	"%s [options] /path/to/region1 ...\n"
	"Options:\n"
	"  -G: defra[G]ment  Removes empty sectors from the file.\n"
	"  -R: [R]epair      Identifies and repairs faults within the file.\n"
	"  -c  [c]heck       Performs more thorough checks on the file.\n"
	"  -q: [q]uiet       Silences most error output.\n"
	"  -v: [v]erbose     Outputs more detailed information.\n"
	"  -V: [V]ersion     Outputs version information & exits.\n"
	"  -h: [h]elp        Shows this output & exits.\n"
	"View %s(1) for more information.\n";
static _Bool signaled = 0;

static void signal_received(int sig)
{
	(void)sig;
	signaled = 1;
}

static int try_ftruncate(int fd, usize size, const char *pat)
{
	int e;
	do e = ftruncate(fd, size);
	while (e && errno == EINTR);
	if (e) {
		warn("%s: ftruncate() failed", pat);
		return 1;
	}
	return 0;
}

/* Processes an .mcX file with the given options.
 * Returns non-zero on failure. */
static int procmcx(char *pat, int opt)
{
	_Bool need_write = opt & OPT_NEED_WRITE;
	usize size, esize, tmp;
	void *mcx;

	const int fd = open(pat, need_write ? O_RDWR : O_RDONLY);
	if (fd < 0) {
		warn("%s", pat);
		goto err;
	}

	struct stat st;
	fstat(fd, &st);
	size = st.st_size;
	if (size < MCX_TABLES) {
		/* Not deleting this, since I do not think it is wise to decide that here. */
		warnx("%s: Too small to contain table (%zuB < %zuB)",
			pat, size, (usize)MCX_TABLES);
		goto err_close;
	}
	tmp = size % MCX_SECTOR;
	if (tmp && !(opt & OPT_QUIET || opt & OPT_CHECK))
		warnx("%s: Not 4KiB sector aligned! (%+zdB)", pat, -tmp);

	mcx = mmap(NULL, size, need_write ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, fd, 0);
	if (mcx == MAP_FAILED) {
		warn("%s", pat);
		goto err_close;
	}

	if (opt & OPT_CHECK)
		mcx_check(mcx, size, pat);

	if (opt & OPT_REPAIR) {
		tmp = mcx_repair(mcx, size);
		if (try_ftruncate(fd, tmp, pat))
			goto err_unmap;
		size = tmp;
	}

	if (opt & OPT_DEFRAG) {
		esize = mcx_calcsize(mcx);
		tmp   = size - esize;
		if ((ssize)tmp < 0) {
			warnx("%s: Predicted a larger size than the actual size. (%+zdB)", pat, tmp);
			goto err_unmap;
		}
		tmp = mcx_defrag(mcx);
		if (try_ftruncate(fd, tmp, pat))
			goto err_unmap;
	}

	munmap(mcx, size);
	close(fd);
	if (opt & OPT_NEED_WRITE && !size) {
		if (remove(pat))
			warn("failed to remove '%s'", pat);
		else if (opt & OPT_VERBOSE)
			printf("removed '%s'\n", pat);
	}
	return 0;
err_unmap:
	munmap(mcx, size);
err_close:
	close(fd);
err:
	return 1;
}

/* Entry-point of the application. */
int main(int argc, char **argv)
{
	/* TODO: With some system calls, we may want to check against
	 * errno == EINTR, and retry the syscall, since no actual error occurred.
	 * This may not be necessary however, considering mmap does not have this problem. */
	signal(SIGINT, signal_received);
	signal(SIGTERM, signal_received);
	argv0 = *argv;

	/* Load the options given in argv. */
	int opt = 0;
	int o;
	while ((o = getopt(argc, argv, "GRcqvVh")) != -1) {
		switch (o) {
		case 'G': opt |= OPT_DEFRAG; break;
		case 'R': opt |= OPT_REPAIR; break;
		case 'c': opt |= OPT_CHECK; break;
		case 'q':
			opt |= OPT_QUIET;
			opt &= ~OPT_VERBOSE;
			break;
		case 'v':
			opt |= OPT_VERBOSE;
			opt &= ~OPT_QUIET;
			break;
		case 'V':
			printf("%s: v%s\n", *argv, MCXEDIT_VERSION);
			return 0;
		case 'h':
			printf(str_help, *argv, *argv);
			return 0;
		case '?':
			fprintf(stderr, "Try '%s -h' for help.\n", *argv);
			return 1;
		default: errx(-SIGTRAP, "Unhandled flag -- '%c', please report this bug.", o);
		}
	}

	/* Loop through the remaining options given in argv (files). */
	int    err = 0;
	char **pat = argv + optind;
	for (; !signaled && *pat; ++pat)
		err |= procmcx(*pat, opt);
	return err;
}
