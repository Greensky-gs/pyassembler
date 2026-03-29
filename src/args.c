#include "args.h"
#include "tools.h"

int in_args(int argc, char * argv[], char * target) {
	int i = 1;
	while (i < argc) {
		if (streq(argv[i], target)) return 1;
		i++;
	}
	
	return 0;
}
char * arg_value(int argc, char * argv[], char * target) {
	int i = 1;
	while (i < argc - 1) {
		if (streq(argv[i], target)) return argv[i + 1];
		i++;
	}
	return NULL;
}

static int parse_int(char * input, int * res) {
	int resultat = 0;
	int i = 0;

	while (input[i] != 0) {
		if (input[i] < 48 || input[i] > 57) return 0;
		resultat = resultat * 10 + (input[i] - 48);
		i++;
	}

	*res = resultat;

	return 1;
}

int get_int(int argc, char * argv[], char * target, int * res) {
	int i = 1;
	while (i < argc - 1) {
		if (streq(argv[i], target)) return parse_int(argv[i + 1], res);
		i++;
	}
	return 2;
}
