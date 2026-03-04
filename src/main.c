/* Copyright (C)2025-2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(__unix__)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "dat/mcx.h"
#include "types.h"
#include "util.h"

_Static_assert(-3 >> 5 == -1,
	"The platform does not compile "
	"right-shifting signed integers to an arithmetic shift!");

const char        *argv0;

/* Entry-point of the application. */
int main(int argc, char **argv)
{
	argv0 = *argv;
	for (int i = 1; i < argc; ++i) {
		int fd = open(argv[i], O_RDWR);
		if (fd < 0) {
			warn("Failed to open '%s'", argv[i]);
			continue;
		}

		usize size, esize, tmp;

		struct stat st;
		fstat(fd, &st);
		size = st.st_size;
		if (size < MCX_TABLES) {
			// warnx("%s: Too small to contain table: (%zuB)", argv[i], size);
			close(fd);
			continue;
		}
		tmp = size % MCX_SECTOR;
		if (tmp) warnx("%s: Not 4KiB sector aligned! (-%zuB)", argv[i], MCX_SECTOR - tmp);

		void *mcx = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (mcx == MAP_FAILED) {
			warn("%s: Failed to map!", argv[i]);
			close(fd);
			continue;
		}

		esize = mcx_calcsize(mcx);
		tmp   = size - esize;
		if ((ssize)tmp < 0)
			warnx("%s: Predicted a larger size than the actual size; file is corrupt! (%zdB)", argv[i], tmp);

		munmap(mcx, size);
		close(fd);
	}

	return 0;
}
