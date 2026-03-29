#include "assembler.h"
#include "list.h"
#include "reader.h"
#include "tools.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

struct assembler_datas {
	chained_cell names;
	int outputfd;
	int fails_count;

	struct assembler_options * options;
};

void write_callback(char * fullpath, void * pointer) {
	if (!py_file(fullpath)) return;

	struct assembler_datas * datas = (struct assembler_datas *)pointer;

	FILE * stream;
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

	write_header(datas->outputfd, fullpath);
	copy_content(datas->outputfd, stream, datas->names, datas->options->max_consecutive_newlines);
	write_footer(datas->outputfd, fullpath);

	fclose(stream);
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

	chained_cell names = reader_scan_names(inputdirname);

	struct assembler_datas data = {
		names,
		outputfd,
		0,
		&options
	};

	recursive_scan(inputdirname, &data, write_callback);

	if (data.fails_count > 0) {
		printf("\x1b[31ERROR\x1b[0m An error occured during copying. Aborting.\n");
		return 1;
	}
	destroy_list(names);

	close(outputfd);
	return 0;
}
