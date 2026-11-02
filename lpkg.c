#include <sys/param.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "config.h"

#if !HAVE_GETPROGNAME
const char *getprogname(void) { return "lpkg"; }
#endif

#define URL "https://github.com/littlefly365/sysrecipes"

static const struct option longopts[] = {
	{ "install", no_argument, 0, 'i' },
	{ "remove", no_argument, 0, 'r' },
	{ "update", no_argument, 0, 'u' },
	{ "upgrade", no_argument, 0, 'U' },
	{ "help", no_argument, 0, 'h' },
	{ "version", no_argument, 0, 'V' },
	{ 0, 0, 0, 0 }
};

enum MODE {
	NONE,
	INSTALL,
	REMOVE,
	UPDATE,
	UPGRADE,
};

static void usage(void);
static void output_version(void);

int
main(int argc, char *argv[])
{
	int c;
	enum MODE mode = NONE;
	while ((c = getopt_long(argc, argv, "iruUhV", longopts, NULL)) != -1) {
		switch (c) {
		case 'i':
			mode = INSTALL;
			break;
		case 'r':
			mode = REMOVE;
			break;
		case 'u':
			mode = UPDATE;
			break;
		case 'U':
			mode = UPGRADE;
			break;
		case 'h':
			usage();
		case 'V':
			output_version();
		default:
			fprintf(stderr, "Try '%s --help' for more information.\n", getprogname());
			return 1;
		}
	}

	argc -= optind;
	argv += optind;

	if (mode == UPDATE) {
		pid_t pid = fork();

		switch (pid) {
		case -1:
			fprintf(stderr, "%s: fork: %s\n", getprogname(), strerror(errno));
			return 1;
		case '0':
			char *const args[] = { "update_repository", "-u", URL, NULL };
			execve("/usr/local/bin/update_repository", args, NULL);

			fprintf(stderr, "%s: execve: %s\n", getprogname(), strerror(errno));
			_exit(127);
		default:
			break;
		}

		int status;
		if (waitpid(pid, &status, 0) < 0) {
			fprintf(stderr, "%s: waitpid: %s\n", getprogname(), strerror(errno));
			return 1;
		}

		if (status != 0) {
			fprintf(stderr, "%s: cannot be possible to update the recipes.\n", getprogname());
			return 1;
		}

		printf("%s: The recipes have been updated successfuly.\n", getprogname());
		return 0;
	}

	return 0;
}

static void
usage(void)
{
	printf(	"Usage: %s [OPTION]... [PACKAGE]...\n"
		"Options:\n"
		"  -i, --install        Install local package(s) into the system\n"
		"  -r, --remove         Remove package(s) from the system\n"
		"  -u, --update         Update the recipes list\n"
		"  -U, --upgrade        Upgrade package(s)\n"
		"  -h, --help           Display this help and exit\n"
		"  -V, --version        Output version information and exit\n"
		,getprogname());
	exit(0);
}

static void
output_version(void)
{
	printf(	"%s (" PROJECT_NAME ") " PROJECT_VERSION "\n"
		"Copyright (C) 2026 littlefly365.\n"
		"License GPLv2: GNU GPL version 2 <https://gnu.org/licenses/gpl.html>.\n"
		"This is free software: you are free to change and redistribute it.\n"
		"There is NO WARRANTY, to the extent permitted by law.\n\n"
		"Written by littlefly365.\n", getprogname());
	exit(0);
}
