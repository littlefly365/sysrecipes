#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>

#include "config.h"

#if !HAVE_GETPROGNAME
const char *getprogname(void) { return "lpkg"; }
#endif

static const struct option longopts[] = {
	{ "help", no_argument, 0, 'h' },
	{ "version", no_argument, 0, 'V' },
	{ 0, 0, 0, 0 }
};

extern char **environ;

enum MODE {
	NONE,
	INSTALL,
	UNINSTALL,
#if 0
	UPDATE,
	UPGRADE,
#endif
	LIST
};

static void usage(void);
static void output_version(void);

int
main(int argc, char *argv[])
{
	int c;
	enum MODE mode = NONE;
	while ((c = getopt_long(argc, argv, "hV", longopts, NULL)) != -1) {
		switch (c) {
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

	if (argc == 0) {
		fprintf(stderr, "%s: no operation specified.\n"
				"Try '%s --help' for more information.\n",
				getprogname(), getprogname());
		return 1;
	}

	if (!strcmp(*argv, "install"))
		mode = INSTALL;
	else if (!strcmp(*argv, "uninstall"))
		mode = UNINSTALL;
#if 0
	else if (!strcmp(*argv, "update"))
		mode = UPDATE;
	else if (!strcmp(*argv, "upgrade"))
		mode = UPGRADE;
#endif
	else if (!strcmp(*argv, "list"))
		mode = LIST;
	else {
		fprintf(stderr, "%s: unknown operation '%s'\n"
				"Try '%s --help' for more information.\n",
				getprogname(), *argv, getprogname());
		return 1;
	}

	argc--; argv++;

	switch (mode) {
	case INSTALL:
	case UNINSTALL:
		pid_t pid = fork();
		switch (pid) {
		case -1:
			fprintf(stderr, "%s: fork: %s\n", getprogname(), strerror(errno));
			return 1;
		case 0:
			const char *path = (mode == INSTALL) ? "/usr/bin/pkg_add" : "/usr/bin/pkg_delete";
			execve(path, argv, environ);
			fprintf(stderr, "%s: %s: %s\n", getprogname(), path, strerror(errno));
			_exit(127);
		default:
			break;
		}

		/*
		 * If the operation is not successfuly pkg_add/pkg_delete already print and error message
		*/

		if (waitpid(pid, NULL, 0) < 0) {
			fprintf(stderr, "%s: waitpid: %s\n", getprogname(), strerror(errno));
			return 1;
		}

		break;
	case LIST:
		DIR *dir;
		struct dirent *ent;

		if ((dir = opendir("/var/lpkg/installed")) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				if (ent->d_name[0] == '.' || ent->d_type != DT_DIR)
					continue;

				puts(ent->d_name);
			}
		} else {
			fprintf(stderr, "%s: no installed packages.\n", getprogname());
			return 1;
		}

		break;
	}

	return 0;
}

static void
usage(void)
{
	printf(	"Usage: %s [OPERATION] <PACKAGE>...\n"
		"or:    %s [OPTION]\n"
		"\nOperations:\n"
		"  install              Install package(s)\n"
		"  uninstall            Uninstall package(s) and its dependencies\n"
#if 0
		"  update               Update the recipes\n"
		"  upgrade              Upgrade installed package(s)\n"
#endif
		"  list                 Print installed packages\n"
		"\nOptions:\n"
		"  -h, --help           Display this help and exit\n"
		"  -V, --version        Output version information and exit\n",
		getprogname());
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
