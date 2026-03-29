#ifndef __READER_H__
#define __READER_H__ 1
#include "list.h"

extern int py_file(char *);

extern void recursive_scan(char * path, void *, void reg_callback(char *, void *));
extern chained_cell reader_scan_names(char * path);

#endif
