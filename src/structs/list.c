#include <stdlib.h>
#include "../aux/tools.h"
#include "list.h"

chained_cell create_cell(char * value) {
	chained_cell cell;
	if ((cell = malloc(sizeof(struct schained_cell))) == NULL) return NULL;

	cell->next = NULL;
	cell->value = value;

	return cell;
}
void destroy_list(chained_cell cell) {
	while (cell != NULL) {
		free(cell->value);
		chained_cell temp = cell->next;

		free(cell);
		cell = temp;
	}
}
int exists_list(chained_cell cell, char * value) {
	while (cell != NULL) {
		if (streq(value, cell->value)) return 1;
		cell = cell->next;
	}
	return 0;
}
int append_list(chained_cell * cell, char * value) {
	chained_cell new;
	if ((new = create_cell(value)) == NULL) return 0;

	if (*cell == NULL) {
		*cell = new;
		return 1;
	}

	chained_cell current = *cell;
	chained_cell next = current->next;
	while (next != NULL) {
		current = next;
		next = current->next;
	}

	current->next = new;

	return 1;
}
int remove_list(chained_cell * list, char * target) {
	if (*list == NULL) return 0;

	if (streq((*list)->value, target)) {
		chained_cell next = (*list)->next;

		free((*list)->value);
		free((*list));

		*list = next;
		return 1;
	}
	chained_cell previous = *list;
	chained_cell curr = (*list)->next;

	while (curr != NULL) {
		if (streq(curr->value, target)) {
			previous->next = curr->next;

			free(curr->value);
			free(curr);

			return 1;
		}
		curr = curr->next;
	}
	return 0;
}
int size_list(chained_cell cell) {
	int i = 0;
	
	while (cell != NULL) {
		cell = cell->next;
		i++;
	}
	return i;
}

void foreach_list(chained_cell cell, void callback(chained_cell)) {
	while (cell != NULL) {
		callback(cell);
		cell = cell->next;
	}
}
