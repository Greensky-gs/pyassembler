#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__ 1

struct assembler_options {
	int max_consecutive_newlines;
	int fullpaths;
};

extern int assemble(char * inputdirname, char * outputname, struct assembler_options);

#endif
