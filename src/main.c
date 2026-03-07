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
	OPT_DRY_RUN = 4,
	OPT_DEFRAG  = 8,
	OPT_REPAIR  = 16,
};

const char        *argv0;
static const char *str_help =
	"%s [options] /path/to/region1 ...\n"
	"Options:\n"
	"  -G: defra[G]ment  Removes empty sectors from the file.\n"
	"  -R: [R]epair      Identifies and repairs faults within the file.\n"
	"  -d: [d]ry-run     Opens the files as read-only.\n"
	"  -h: [h]elp        Shows this output & exits.\n"
	"  -q: [q]uiet       Silences most error output.\n"
	"  -v: [v]erbose     Outputs more detailed information.\n"
	"View %s(1) for more information.\n";
static _Bool signaled = 0;

static void signal_received(int sig)
{
	signaled = 1;
}

/* Entry-point of the application. */
int main(int argc, char **argv)
{
	/* TODO: With some system calls, we may want to check against
	 * errno == EINTR, and retry the syscall, since no actual error occurred.
	 * This may not be necessary however, considering mmap does not have this problem. */
	signal(SIGINT, signal_received);
	signal(SIGTERM, signal_received);
	int opt = 0;
	argv0   = *argv;
	int o;
	while ((o = getopt(argc, argv, "GRdhqv")) != -1) {
		switch (o) {
		case 'G': opt |= OPT_DEFRAG; break;
		case 'R': opt |= OPT_REPAIR; break;
		case 'd': opt |= OPT_DRY_RUN; break;
		case 'q':
			opt |= OPT_QUIET;
			opt &= ~OPT_VERBOSE;
			break;
		case 'v':
			opt |= OPT_VERBOSE;
			opt &= ~OPT_QUIET;
			break;
		case 'h':
			printf(str_help, *argv, *argv);
			return 0;
		case '?':
			fprintf(stderr, "Try '%s -h' for help.\n", *argv);
			return 1;
		default: UNREACHABLE();
		}
	}

	/* Process the *.mcX files */
	for (int i = optind; i < argc; ++i) {
		int fd = open(argv[i], opt & OPT_DRY_RUN ? O_RDONLY : O_RDWR);
		if (fd < 0) {
			warn("Failed to open '%s'", argv[i]);
			continue;
		}

		usize size, esize, tmp;

		struct stat st;
		fstat(fd, &st);
		size = st.st_size;
		if (size < MCX_TABLES) {
			warnx("%s: Too small to contain table: (%zuB)", argv[i], size);
			close(fd);
			continue;
		}
		tmp = size % MCX_SECTOR;
		if (tmp && !(opt & OPT_QUIET))
			warnx("%s: Not 4KiB sector aligned! (-%zuB)", argv[i], MCX_SECTOR - tmp);

		void *mcx = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (mcx == MAP_FAILED) {
			warn("%s: Failed to map!", argv[i]);
			close(fd);
			continue;
		}

		esize = mcx_calcsize(mcx);
		tmp   = size - esize;
		if ((ssize)tmp < 0 && opt & OPT_QUIET)
			warnx("%s: Predicted a larger size than the actual size; file is corrupt! (%zdB)", argv[i], tmp);

		if (opt & OPT_DEFRAG) tmp = mcx_defrag(mcx);
		if (opt & OPT_REPAIR) tmp = mcx_repair(mcx, size);
		if (opt & OPT_VERBOSE && opt & (OPT_DEFRAG | OPT_REPAIR))
			printf("%s: New size is (%zdB)", argv[i], size - tmp);


		munmap(mcx, size);
		close(fd);
	}

	return 0;
}
