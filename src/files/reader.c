#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "reader.h"
#include "../aux/tools.h"

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

struct scan_names_callback_data {
	chained_cell * chained_list;
	int origin_size;
};
void scan_names_callback(char * fullpath, void * pointer) {
	if (!py_file(fullpath)) return;
	struct scan_names_callback_data * data = (struct scan_names_callback_data *)pointer;
	chained_cell * list = data->chained_list;

	int i = 0;
	char * slash = fullpath + data->origin_size + 1;

	while (slash[i] != 0) {
		if (slash[i] == '/') slash[i] = '.';
		i++;
	}

	char * filename = slash;
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

void import_names_callback(char * path, void * pointer) {
	chained_cell * listp = (chained_cell *)pointer;

	FILE * stream;
	if ((stream = fopen(path, "rt")) == NULL) return;

	char buffer[BUFSIZ] = {0};
	while (fgets(buffer, BUFSIZ - 1, stream) != NULL) {
		char * copy;
		int size;
		char ** words;
		
		if ((words = extract_words(buffer, &size, &copy)) == NULL) continue;

		int i = 0;
		while (i < size - 1) {
			if (streq(words[i], "from")) {
				char * word = words[i + 1];
				if (listp == NULL || !exists_list(*listp, word)) {
					char * value;
					int totalsize = strlen(word) + strlen(buffer) + 2; // Boar thechnique, we will concatenate two strings in one malloc, with 2 \0
					if ((value = malloc(totalsize)) == NULL) break;

					int i = 0;
					while (word[i] != 0) {
						value[i] = word[i];
						i++;
					}
					value[i] = 0;
					i++;

					int j = 0;
					while (buffer[j] != 0) {
						value[i + j] = buffer[j];
						j++;
					}
					value[j + i] = 0;

					append_list(listp, value);
				}
				break;
			}
			i++;
		}

		free(copy);
		free(words);
	}

	fclose(stream);
}

chained_cell get_all_import_names(char * inputpath) {
	chained_cell list = NULL;

	recursive_scan(inputpath, &list, import_names_callback);

	return list;
}

struct perform_datastruct {
	chained_cell * names;
	chained_cell * imports;

	int originsize;
};
void perform_callback(char * path, void * pointer) {
	struct perform_datastruct * data = (struct perform_datastruct *)pointer;

	import_names_callback(path, data->imports);

	struct scan_names_callback_data names_data = {0};

	names_data.chained_list = data->names;
	names_data.origin_size = data->originsize;
	scan_names_callback(path, &names_data);
}

int perform_scans(char * path, chained_cell * names, chained_cell * imports) {
	struct perform_datastruct data = {
		names,
		imports,
		strlen(path)
	};

	recursive_scan(path, &data, perform_callback);

	return 0;
}

