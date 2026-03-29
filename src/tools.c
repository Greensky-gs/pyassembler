#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "tools.h"

#ifndef COPY_CONTENT_CHUNK_SIZE
#define COPY_CONTENT_CHUNK_SIZE 512
#endif

int write_header(int fd_dest, char * name) {
	char text[] = "# PyAssembler::start: ";
	char * temp;

	int totalsize = strlen(text) + strlen(name) + 2;
	if ((temp = malloc(totalsize + 1)) == NULL) return 1;

	snprintf(temp, totalsize + 1, "%s%s\n\n", text, name);
	write(fd_dest, temp, totalsize);

	free(temp);
	return 0;
}
int write_footer(int fd_dest, char * name) {
	char text[] = "\n# PyAssembler::end: ";
	char * temp;

	int totalsize = strlen(text) + strlen(name) + 1;
	if ((temp = malloc(totalsize + 1)) == NULL) return 1;

	snprintf(temp, totalsize + 1, "%s%s\n", text, name);
	write(fd_dest, temp, totalsize);

	free(temp);
	return 0;
}

int is_path_nav(char * path) {
	if (path[0] == 0) return 0;
	if (path[0] == '.' && path[1] == 0) return 1;
	if (path[0] == '.' && path[1] == '.' && path[2] == 0) return 1;
	return 0;
}

int streq(char * a, char * b) {
	int i = 0;
	while (a[i] != 0 && b[i] != 0) {
		if (a[i] != b[i]) return 0;
		i++;
	}

	return a[i] == b[i];
}

char * malloc_name(char * path) {
	int i = 0;
	char * last_dot = path;
	while (path[i] != 0) {
		if (path[i] == '.') last_dot = path + i;
		i++;
	}

	int length = last_dot - path;
	if (length == 0) return NULL;

	char * val;
	if ((val = malloc(length + 1)) == NULL) return NULL;

	int j = 0;
	while (j < length) {
		val[j] = path[j];
		j++;
	}

	val[length] = 0;
	return val;
}

char ** extract_words(char * input, int * p, char ** catch_string) {
	char * copy;
	if ((copy = malloc(strlen(input) + 1)) == NULL) return NULL;
	strcpy(copy, input);
	copy[strlen(input)] = 0;

	int i = 0;
	*p = 0;
	int in_word = 0;
	while (input[i] != 0) {
		if (input[i] == ' ') {
			in_word = 0;
		} else if (!in_word) {
			(*p)++;
			in_word = 1;
		}
		i++;
	}

	char ** rep;
	if ((rep = malloc(*p * sizeof(char *))) == NULL) {
		free(copy);
		return NULL;
	}
	
	int j = 0;
	in_word = 0;
	int index = 0;
	while (input[j] != 0) {
		if (input[j] == ' ' && in_word) {
			copy[j] = 0;
			in_word = 0;
		} else if (input[j] == ' ' && !in_word) {
			in_word = 0;
		} else if (!in_word) {
			in_word = 1;
			rep[index++] = copy + j;
		}
		j++;
	}

	*catch_string = copy;

	return rep;
}

static char * find_import(char ** words, int size) {
	int i = 0;
	while (i < size - 1) {
		if (streq(words[i], "from")) return words[i + 1];
		i++;
	}
	return NULL;
}

int copy_content(int fd_dest, FILE * input, chained_cell imports, int max_consec) {
	char line[BUFSIZ];

	int consecutives = 0;
	while (fgets(line, BUFSIZ - 1, input) != NULL) {
		char ** words;
		char * copy = NULL;
		int size;
		if ((words = extract_words(line, &size, &copy)) == NULL) continue;

		char * importname;
		if ((importname = find_import(words, size)) != NULL && exists_list(imports, importname)) {
			free(copy);
			free(words);
			continue;
		}
		int newline = line[0] == '\n';
		if (newline) {
			consecutives++;
			if (consecutives > max_consec - 1) {
				free(copy);
				free(words);
				continue;
			}
		} else {
			consecutives = 0;
		}

		write(fd_dest, line, strlen(line));

		free(copy);
		free(words);
	}

	return 0;
}

int exec_command(char * args[]) {
	pid_t pid;
	int status;

	switch (pid = fork()) {
		case -1:
			perror("fork");
			return -1;
		case 0:
			execvp(args[0], args);
			perror("execvp");
			exit(127);
			break;
		default:
			waitpid(pid, &status, 0);

			if (WIFEXITED(status)) return WEXITSTATUS(status);
			return 128;
	}
}

int output_prelude(char * dest, char ** ref) {
	// Let's create the directory if necessary
	int i = 0;
	char * slash = dest;

	while (dest[i] != 0) {
		if (dest[i] == '/' && dest[i + 1] != 0) slash = dest + i;
		i++;
	}

	if (ref != NULL) *ref = slash;

	*slash = 0;
	char * args[] = { "mkdir", "-p", dest, NULL };

	if (exec_command(args) != 0) {
		*slash = '/';
		return 1;
	}
	*slash = '/';
	return 0;
}
