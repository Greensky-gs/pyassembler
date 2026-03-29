#ifndef __TOOLS_H__
#define __TOOLS_H__ 1
#include "list.h"
#include <stdio.h>

extern int write_header(int fd_dest, char * name);
extern int write_footer(int fd_dest, char * name);
extern int write_imports_start(int fd_dest);
extern int write_imports_end(int fd_dest);
extern int write_imports_list(int fd_dest, chained_cell imports);

extern int is_path_nav(char * path);
extern int streq(char *, char *);
extern char * malloc_name(char * path);

extern char ** extract_words(char * input, int * psize, char ** catch_string); // Reads from input, and mallocs a pointer of pointers to places a string set in catch_string 

extern int copy_content(int fd_dest, FILE *, int max_consec_newlines);

extern int exec_command(char *[]);
extern int output_prelude(char *, char ** _Nullable);
extern char * next_zero(char *);

#endif
