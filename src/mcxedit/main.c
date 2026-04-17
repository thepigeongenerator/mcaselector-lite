/* Copyright (C)2025-2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#include "main.h"

#include "err.h"
#include "getopt.h"
#include <errno.h>
#include <libmcx/mcx.h>
#include <libmcx/types.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__unix__)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#elif defined(_WIN32)
/* TODO: Windows implementation. */
#else
#error "Platform unsupported"
#endif

enum options {
	OPT_VERBOSE = 1,
	OPT_QUIET   = 2,
	OPT_CHECK   = 4,
	OPT_DEFRAG  = 8,
	OPT_REPAIR  = 16,

	/* Options that need write access to the file. */
	OPT_NEED_WRITE = OPT_DEFRAG | OPT_REPAIR,
};

const char *argv0;
static int  signaled = 0;

struct file {
#if defined(__unix__)
	int fd;
#elif _WIN32
/* TODO: Windows implementation. */
#else
#error "Platform unsupported"
#endif
};

/* Closes a file.
 * Returns 0 if successful, or -1 upon failure. */
static int file_close(const struct file *f)
{
#if defined(__unix__)
	int e;
	do {
		errno = 0;
		e     = close(f->fd);
	} while (e < 0 && errno == EINTR);
	return e; /* e = 0 || -1 */
#elif defined(_WIN32)
/* TODO: Windows implementation. */
#else
#error "Platform unsupported"
#endif
}

/* Opens a file.
 * "pat" specifies the file-path.
 * need_write sets whether the file is writeable, or read-only.
 * Returns the file size, or -1 upon failure. */
static off_t file_open(struct file *f, const char *pat, int need_write)
{
#if defined(__unix__)
	f->fd = open(pat, need_write ? O_RDWR : O_RDONLY);
	if (f->fd < 0)
		goto err_open;
	struct stat st;
	if (fstat(f->fd, &st) < 0)
		goto err_stat;
	return st.st_size;
#elif defined(_WIN32)
/* TODO: Windows implementation. */
#else
#error "Platform unsupported"
#endif
err_open:
	warn("cannot open '%s'", pat);
	return -1;
err_stat:
	warn("cannot stat '%s'", pat);
	file_close(f);
	return -1;
}

/* Maps "size" in bytes from *f to a memory address.
 * "need_write" dictates whether the mapping is writeable or read-only.
 * Returns (void *)-1 upon failure. (NULL is possibly a valid value) */
static void *file_map(const struct file *f, off_t size, int need_write)
{
	void *mcx;
#if defined(__unix__)
	int map_prot = need_write ? (PROT_READ | PROT_WRITE) : PROT_READ;
	mcx          = mmap(NULL, size, map_prot, MAP_SHARED, f->fd, 0);
#elif defined(_WIN32)
/* TODO: Windows implementation. */
#else
#error "Platform unsupported"
#endif
	return mcx;
}

/* Removes the mapping from created with "file_map".
 * "size" should be the amount of bytes that we're unmapping.
 * Returns 0 if successful, or -1 upon failure. */
static int file_unmap(const struct file *f, void *mcx, off_t size)
{
#if defined(__unix__)
	return munmap(mcx, size);
#elif defined(_WIN32)
/* TODO: Windows implementation. */
#else
#error "Platform unsupported"
#endif
}

/* Truncates file *f to a specified size.
 * Returns 0 if successful, or -1 upon failure. */
static int file_truncate(const struct file *f, off_t size)
{
	int e;
#if defined(__unix__)
	do e = ftruncate(f->fd, size);
	while (e < 0 && errno == EINTR); /* e = 0 or -1*/
#elif defined(_WIN32)
/* TODO: Windows implementation. */
#else
#error "Platform unsupported"
#endif
	return e;
}

/* Processes an .mcX file with the given options.
 * Returns non-zero on failure. */
static int procmcx(const char *pat, int opt)
{
	struct file f;

	int   need_write = opt & OPT_NEED_WRITE;
	off_t size, nsize, tmp;
	void *mcx;

	/* Open the file, get and check the size. */
	size = file_open(&f, pat, need_write);
	if (size < 0) goto err;
	if (size < MCX_TABLES) {
		size = 0;
		if (opt & OPT_REPAIR || opt & OPT_DEFRAG)
			goto suc_close; /* Delete when performing the above. */
		warnx("cannot use '%s': Too small to contain table (%lluB < %lluB)",
			pat, (ullong)size, (ullong)MCX_TABLES);
		goto err_close;
	}

	tmp = size % MCX_SECTOR;
	if (tmp && !(opt & OPT_QUIET || opt & OPT_CHECK)) {
		warnx("'%s' may be corrupt: Not 4KiB sector aligned! (%+lldB)",
			pat, (intmax_t)-tmp);
	}
	if ((uintmax_t)size > SIZE_MAX) {
		warnx("cannot use '%s': File is too large for the size_t datatype", pat);
		goto err_close;
	}

	mcx = file_map(&f, size, need_write);
	if (mcx == (void *)-1) {
		warn("cannot map '%s'", pat);
		goto err_close;
	}

	/* Perform the requested actions from opt. */
	if (opt & OPT_CHECK) {
		off_t calcsize = mcx_calcsize(mcx);
		off_t sumsize  = mcx_sumsize(mcx);
		if (size < calcsize)
			warnx("%s: Contains chunks that exeed file size! (%lluB < %lluB)",
				pat, (ullong)size, (ullong)calcsize);
		else if (size < sumsize)
			warnx("%s: Sum of chunk sizes exeed file size! (%lluB < %lluB)",
				pat, (ullong)size, (ullong)sumsize);
	}

	if (opt & OPT_REPAIR) {
		nsize = mcx_repair(mcx, size);
		if (file_truncate(&f, nsize)) {
			warn("cannot truncate '%s'", pat);
			goto err_unmap;
		}
		size = nsize;
	}

	if (opt & OPT_DEFRAG) {
		nsize = mcx_defrag(mcx, size);
		if (file_truncate(&f, nsize)) {
			warn("cannot truncate '%s'", pat);
			goto err_unmap;
		}
		size = nsize;
	}

	if (file_unmap(&f, mcx, size) < 0)
		err(1, "cannot unmap '%s'", pat);
suc_close:
	if (file_close(&f) < 0)
		err(1, "cannot close '%s'", pat);
	if (opt & OPT_NEED_WRITE && !size) {
		if (remove(pat))
			warn("cannot remove '%s'", pat);
		else if (opt & OPT_VERBOSE)
			printf("removed '%s'\n", pat);
	}

	return 0;
err_unmap:
	if (file_unmap(&f, mcx, size) < 0)
		err(1, "cannot unmap '%s'", pat);
err_close:
	if (file_close(&f) < 0)
		err(1, "cannot close '%s'", pat);
err:
	return 1;
}

static void signal_report(int sig)
{
	signaled = sig;
}

/* Entry-point of the application. */
int main(int argc, char **argv)
{
	signal(SIGINT, signal_report);
	signal(SIGTERM, signal_report);
	argv0 = *argv;

	/* Load the options given in argv. */
	int opt = 0;
	int o;
	while ((o = getopt(argc, argv, "GRcqvVh")) != -1) {
		switch (o) {
		case 'G': opt |= OPT_DEFRAG; break;
		case 'R': opt |= OPT_REPAIR; break;
		case 'c': opt |= OPT_CHECK; break;
		case 'q': opt |= OPT_QUIET; break;
		case 'v': opt |= OPT_VERBOSE; break;
		case 'V':
			printf("%s: %s\n", *argv, MCXEDIT_VERSION);
			return 0;
		case 'h':
			printf(
				"%s [options] /path/to/region1 ...\n"
				"Options:\n"
				"  -G: defra[G]ment  Removes empty sectors from the file.\n"
				"  -R: [R]epair      Identifies and repairs faults within the file.\n"
				"  -c  [c]heck       Performs more thorough checks on the file.\n"
				"  -q: [q]uiet       Silences most error output.\n"
				"  -v: [v]erbose     Outputs more detailed information.\n"
				"  -V: [V]ersion     Outputs version information & exits.\n"
				"  -h: [h]elp        Shows this output & exits.\n"
				"View mcxedit(1) for more information.\n",
				*argv);
			return 0;
		case '?':
			fprintf(stderr, "Try '%s -h' for help.\n", *argv);
			return 1;
		default: errx(-SIGABRT, "Unhandled flag -- '%c', please report this bug.", o);
		}
	}

	/* Loop through the remaining options given in argv (files). */
	int    err = 0;
	char **pat = argv + optind;
	while (!signaled && *pat)
		err |= procmcx(*pat++, opt);
	return signaled ? signaled + 128 : err;
}
