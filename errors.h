#ifndef __errors_h
#define __errors_h
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define macro that can be used for diagnostic output from examples. when compiled -DDEBUG it resutls in calling printf
// with the specified argument. // WHen Debug is not defined, expands to nothing

#ifdef DEBUG
#define DPRINTF(arg) printf arg
#else
#define DPRINTF(arg)
#endif
#define err_abort(code, text) do {\
	fprintf(stderr, "%s at \"%s\":%d: %s\n", \
			text, __FILE__,__LINE__, strerror(code));\
	abort(); \
} while (0)

#define error_abort(text) do {\
	fprintf(stderr, "%s at \"%s\":%d:%s\n",\
			text, __FILE__, __LINE__, strerror(errno);\
			abort();\
			}while(0)
#endif

// gcc -Wno-implicit -DDEBUG crew.c -o hello -lpthread
// gcc -w -DDEBUG crew.c -o hello -lpthread
//./hello cat /home/oddo/Dropbox/Public/c11/Test/

