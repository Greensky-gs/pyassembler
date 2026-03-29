#ifndef __LIST_H__
#define __LIST_H__ 1

struct schained_cell {
	char * value;
	struct schained_cell * next;
};
typedef struct schained_cell * chained_cell;

extern chained_cell create_cell(char *);
extern void destroy_list(chained_cell);
extern int append_list(chained_cell *, char *);
extern int exists_list(chained_cell, char *);

extern void foreach_list(chained_cell, void callback(chained_cell));

#endif
