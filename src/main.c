#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "aux/args.h"
#include "assembler.h"
#include "aux/tools.h"

void help_page(char * name) {
	printf("Usage: \x1b[90m%s <input directory> -o output/file [OPTIONS]\x1b[0m\n  Options are : \n    --max-newlines <int> : number of consecutives newlines to include in the final py file\n    --no-comments : Don't comment the output\n    --last-file <FILE> : last file to assemble, if found\n", name);
}

int main(int argc, char * argv[]) {
	if (argc < 2) {
		help_page(argv[0]);
		return 1;
	}

	struct arg_input args_list[] = {
		{ "-o", String, 0, 0, "output/main.py" },
		{ "--no-comments", Presence, 0, 0, NULL },
		{ "--verbose", Presence, 0, 0, NULL },
		{ "-v", Presence, 0, 0, NULL },
		{ "--last-file", String, 0, 0, NULL },
		{ "--max-newlines", Int, 0, 0, NULL },
		{ "--fullpaths", Presence, 0, 0, NULL }
	};
	int size = sizeof(args_list) / sizeof(struct arg_input);
	find_all(argc, argv, args_list, size);

	struct assembler_options opts = {0};

	opts.print_comments = args_list[1].found ? 0 : 1;
	opts.fullpaths = args_list[6].found;
	opts.max_consecutive_newlines = args_list[5].int_res;
	opts.last_file = args_list[4].str_result;
	opts.verbose = (args_list[2].found || args_list[3].found) ? 1 : 0;

	char * output = args_list[0].str_result;

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
