#ifndef __READER_H__
#define __READER_H__ 1
#include "../structs/list.h"

extern int py_file(char *);

extern void recursive_scan(char * path, void *, void reg_callback(char *, void *));
extern chained_cell reader_scan_names(char * path);
extern chained_cell get_all_import_names(char * path);

extern int perform_scans(char * input, chained_cell * names, chained_cell * imports);

#endif
