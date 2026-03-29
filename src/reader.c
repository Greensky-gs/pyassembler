#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "reader.h"
#include "tools.h"

int py_file(char * path) {
	int i = 0;
	char * last_dot = path;
	while (path[i] != 0) {
		if (path[i] == '.') last_dot = path + i;
		i++;
	}

	return streq(last_dot, ".py");
}

void recursive_scan(char * path, void * util_pointer, void reg_callback(char * fullpath, void * util_pointer)) {
	DIR * dir;
	if ((dir = opendir(path)) == NULL) return;

	struct dirent * entry;
	struct stat sb;
	while ((entry = readdir(dir)) != NULL) {
		if (is_path_nav(entry->d_name)) continue;

		char * newpath;
		int size = strlen(path) + strlen(entry->d_name) + 1;
		if ((newpath = malloc(size + 1)) == NULL) continue;

		snprintf(newpath, size + 1, "%s/%s", path, entry->d_name);
		if (lstat(newpath, &sb) == -1) {
			free(newpath);
			continue;
		}

		if (S_ISDIR(sb.st_mode)) {
			recursive_scan(newpath, util_pointer, reg_callback);
		} else if (S_ISREG(sb.st_mode)) {
			reg_callback(newpath, util_pointer);
		}

		free(newpath);
	}

	closedir(dir);
}

void scan_names_callback(char * fullpath, void * pointer) {
	if (!py_file(fullpath)) return;
	chained_cell * list = (chained_cell *)pointer;

	int i = 0;
	char * slash = fullpath;
	while (fullpath[i] != 0) {
		if (fullpath[i] == '/') slash = fullpath + i;
		i++;
	}

	char * filename = slash + 1;
	char * dot = filename;
	i =  0;
	while (filename[i] != 0) {
		if (filename[i] == '.') dot = filename + i;
		i++;
	}

	if (dot != filename) *dot = 0;
	int size = strlen(filename);
	char * copy;
	if ((copy = malloc(size + 1)) == NULL) return;
	strcpy(copy, filename);
	copy[size] = 0;

	if (!exists_list(*list, filename)) append_list(list, copy);
}

chained_cell reader_scan_names(char * entry) {
	chained_cell list = NULL;

	recursive_scan(entry, &list, scan_names_callback);

	return list;
}
