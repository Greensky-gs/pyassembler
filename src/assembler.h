#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__ 1
#include "structs/list.h"

struct assembler_options {
	int max_consecutive_newlines;
	int fullpaths;
	char print_comments;
	char * last_file;
};

extern int assemble(char * inputdirname, char * outputname, struct assembler_options);
extern void filter_lists(chained_cell * imports, chained_cell names);

#endif
