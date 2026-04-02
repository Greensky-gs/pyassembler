#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__ 1
#include "structs/list.h"

#define VerboseSuccess "\x1b[32m"
#define VerboseInfo "\x1b[35m"
#define VerboseUseless "\x1b[90m"
#define VerboseCritical "\x1b[31m"
#define VerboseWarning "\x1b[33m"

#ifndef DISABLE_VERBOSE
#define DISABLE_VERBOSE 0
#endif

#if DISABLE_VERBOSE==0
#define IF_VERBOSE(var, mode, name, print_instruction) if ((var)->verbose) {printf("%s[%s]\x1b[0m  ", mode, name);print_instruction;}
#else
#define IF_VERBOSE(var, mode, name, print_instruction) {};
#endif

struct assembler_options {
	int max_consecutive_newlines;
	int fullpaths;
	char print_comments;
	char * last_file;
	char verbose;
};

extern int assemble(char * inputdirname, char * outputname, struct assembler_options);
extern void filter_lists(chained_cell * imports, chained_cell names);

#endif
