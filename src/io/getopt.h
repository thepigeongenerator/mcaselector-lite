/* Copyright (C)2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#ifndef MCXEDIT_OPTS_H
#define MCXEDIT_OPTS_H 1

/* Contains a pointer to the argument value of getopt, or NULL. */
extern char *optarg;

/* When set to zero, error messages are suppressed for getopt. */
extern int opterr;

/* The index of the argv[] vector, containing the element to be processed. */
extern int optind;

/* Contains the option value that an error has occurred on. */
extern int optopt;

/* Processes options grom argc and argv. argv is modified so
 * non-options from the opts string will appear after the options.
 * opts defines the valid options as a string of characters. If an option
 * is suffixed with ':', this option takes a parameter.
 * Returns the option that was given, on failure due to
 * unrecognised options '?' is returned. If opts starts with ':', the function
 * will not print to stderr,
 * and return ':' instead of '?' for argument failures.
 * NOTE: Due to POSIX compatibility argv is marked as CONST, this is false.
 */
int getopt(int argc, char *const *argv, const char *opts);

#endif
