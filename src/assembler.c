#include "assembler.h"
#include "structs/list.h"
#include "files/reader.h"
#include "aux/tools.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

struct assembler_datas {
	chained_cell names;
	int outputfd;
	int fails_count;
	int found_last;

	struct assembler_options * options;
};

void write_callback(char * fullpath, void * pointer) {
	if (!py_file(fullpath)) return;
	
	struct assembler_datas * datas = (struct assembler_datas *)pointer;

	if (datas->options->last_file != 0 && streq(fullpath, datas->options->last_file) && !datas->found_last) {
		datas->found_last = 1;
		return;
	}

	FILE * stream;
	IF_VERBOSE(datas->options, VerboseInfo, "FILEOPEN", printf("Opening \x1b[33m%s\x1b[0m\n", fullpath));

	if ((stream = fopen(fullpath, "rt")) == NULL) {
		datas->fails_count++;
		printf("\x1b[91m ERROR\x1b[0m cannot open \x1b[33m%s\x1b[0m for reading.\n", fullpath);
		return;
	};

	if (!datas->options->fullpaths) {
		int i = 0;
		char * slash = fullpath;
		while (fullpath[i] != 0) {
			if (fullpath[i] == '/') slash = fullpath + i;
			i++;
		}

		fullpath = slash + 1;
	}

	if (datas->options->print_comments == 1) write_header(datas->outputfd, fullpath);
	copy_content(datas->outputfd, stream, datas->options->max_consecutive_newlines);
	if (datas->options->print_comments == 1) write_footer(datas->outputfd, fullpath);

	fclose(stream);
}

void filter_lists(chained_cell * imports, chained_cell names) {
	chained_cell curr = *imports;

	while (curr != NULL) {
		if (exists_list(names, curr->value)) remove_list(imports, curr->value);
		
		curr = curr->next;
	}
}

void display(chained_cell cell) {
	printf("%s\n", cell->value);
}
int assemble(char * inputdirname, char * outputfilename, struct assembler_options options) {
	struct stat sb;
	if (lstat(inputdirname, &sb) == -1) {
		printf("Something went wrong trying to read \x1b[33m%s\x1b[0m. Perhaps it doesn't exist\n", inputdirname);
		return 1;
	}
	if (!S_ISDIR(sb.st_mode)) {
		printf("\x1b[33m%s\x1b[0m is not a directory.\n", inputdirname);
		return 1;
	}

	int outputfd;
	if ((outputfd = open(outputfilename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR)) == -1) {
		printf("\x1b[31mERROR\x1b[0m cannot open \x1b[33m%s\x1b[0m for writing.\n", outputfilename);
		return 1;
	};

	chained_cell names = NULL, imports = NULL;

	IF_VERBOSE(&options, VerboseInfo, "SCANNING", printf("Starting scan for imports...\n"))
	perform_scans(inputdirname, &names, &imports);
	IF_VERBOSE(&options, VerboseUseless, "SCANNING", printf("Found \x1b[33m%d\x1b[0m for \x1b[92mnames\x1b[0m and \x1b[33m%d\x1b[0m for \x1b[92mregular imports\x1b[0m\n", size_list(names), size_list(imports)));
	IF_VERBOSE(&options, VerboseInfo, "SCANNING", printf("Filtering results...\n"))
	filter_lists(&imports, names);
	IF_VERBOSE(&options, VerboseUseless, "SCANNING", printf("Now having \x1b[33m%d\x1b[0m for \x1b[92mnames\x1b[0m and \x1b[33m%d\x1b[0m for \x1b[92mregular imports\x1b[0m\n", size_list(names), size_list(imports)));

	if (options.last_file != NULL) {
		char * fullpath;
		if ((fullpath = calloc(strlen(options.last_file) + strlen(inputdirname) + 2, sizeof(char))) == NULL) {
			printf("\x1b[31mERREUR\x1b[0m allocation de mémoire\n");
			return 1;
		}

		strcat(fullpath, inputdirname);
		strcat(fullpath, "/");
		strcat(fullpath, options.last_file);
		IF_VERBOSE(&options, VerboseInfo, "OPTIONS", printf("found \x1b[33m%s\x1b[0m as last file to assemble\n", fullpath));
		options.last_file = fullpath;
	}

	struct assembler_datas data = {
		names,
		outputfd,
		0,
		0,
		&options
	};

	IF_VERBOSE(&options, VerboseUseless, "WRITING", printf("Starting to write \x1b[33m%d\x1b[0m standard imports\n", size_list(imports)))
	if (options.print_comments == 1) write_imports_start(outputfd);
	write_imports_list(outputfd, imports);
	if (options.print_comments == 1) write_imports_end(outputfd);

	recursive_scan(inputdirname, &data, write_callback);

	if (data.found_last) write_callback(options.last_file, &data);

	if (options.last_file != NULL) free(options.last_file);

	if (data.fails_count > 0) {
		printf("\x1b[31ERROR\x1b[0m An error occured during copying. Aborting.\n");
		return 1;
	}
	destroy_list(imports);
	destroy_list(names);

	IF_VERBOSE(&options, VerboseSuccess, "WRITING", printf("Wrote to \x1b[33m%s\x1b[0m\n", outputfilename))

	close(outputfd);
	return 0;
}
