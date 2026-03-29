#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "args.h"
#include "assembler.h"
#include "tools.h"

void help_page(char * name) {
	printf("Usage: \x1b[90m%s <input directory> -o output/file [OPTIONS]\x1b[0m\n  Options are : \n    --max-newlines <int> : number of consecutives newlines to include in the final py file\n", name);
}

int main(int argc, char * argv[]) {
	if (argc < 2) {
		help_page(argv[0]);
		return 1;
	}

	struct assembler_options opts = {0};
	opts.fullpaths = in_args(argc, argv, "--full");
	opts.max_consecutive_newlines = 1;

	if (get_int(argc, argv, "--max-newlines", &(opts.max_consecutive_newlines)) == 0) {
		printf("Invalid number for \x1b[90m--max-newlines\x1b[0m : integer expected\n");
		return 1;
	}

	char * output = arg_value(argc, argv, "-o");
	if (output == NULL) output = "output/main.py";

	char * copy;
	if ((copy = strdup(output)) == NULL) {
		printf("Something went wrong during the memory allocation. Please try again\n");
		return 1;
	}

	char * slash;
	if (output_prelude(copy, &slash) != 0) {
		*slash = 0;
		printf("Failed to create the \x1b[33m%s\x1b[0m directory\n", copy);
		*slash = '/';

		free(copy);
		return 1;
	}
	free(copy);

	assemble(argv[1], output, opts);

	return 0;
}
