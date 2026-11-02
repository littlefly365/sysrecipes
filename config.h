#ifndef _CONFIG_H
#define _CONFIG_H	1

#define PROJECT_NAME		"lfs-utils"
#define PROJECT_VERSION		"0.1"

#if defined(__linux__)
 #if defined(__NetBSD__)
 #define HAVE_GETPROGNAME	1
 #else
 #define HAVE_GETPROGNAME	0
 #endif
#endif

#endif
