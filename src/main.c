#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "aux/args.h"
#include "assembler.h"
#include "aux/tools.h"

static void help_page(char * name, struct arg_input args[], int size) {
	printf("Usage: \x1b[90m%s <input directory> [OPTIONS]\x1b[0m\n  Available options are:\n", name);

	int i = 0;
	while (i < size) {
		printf("    \x1b[90m%-15s\x1b[0m : %s. Of type \x1b[91m", args[i].name, args[i].description);
		switch (args[i].type) {
			case String:
				printf("string\x1b[0m");
				if (args[i].str_result == NULL) printf(", no default value is provided\n");
				else printf(", default value is set to \x1b[33m%s\x1b[0m\n", args[i].str_result);
				break;
			case Int:
				printf("integer\x1b[0m, default value is set to \x1b[33m%d\x1b[0m\n", args[i].int_res);
				break;
			case Presence:
				printf("toggle\x1b[0m\n");
				break;
		}

		i++;
	}

	printf("\nPyAssembler by \x1b[90mGreensky\x1b[0m at \x1b[94mhttps://github.com/Greensky-gs/pyassembler\x1b[0m\n");
}

int main(int argc, char * argv[]) {
	struct arg_input args_list[] = {
		{ "-o", "Output file path", String, 0, 0, "output/main.py" },
		{ "--no-comments", "Disable assembler comments in output file", Presence, 0, 0, NULL },
		{ "--verbose", "Show details of compiler", Presence, 0, 0, NULL },
		{ "-v", "Alias for --verbose", Presence, 0, 0, NULL },
		{ "--last-file", "Last file to assemble. This file will be remembered and only put at the end", String, 0, 0, NULL },
		{ "--max-newlines", "Max consecutives empty lines to include", Int, 0, 0, NULL },
		{ "--fullpaths", "Show full paths in assembler comments", Presence, 0, 0, NULL }
	};
	int size = sizeof(args_list) / sizeof(struct arg_input);

	if (argc < 2) {
		help_page(argv[0], args_list, size);
		return 1;
	}

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
