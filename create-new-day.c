#include <stdio.h>
#include <stdlib.h>
#include "prelude.h"

int main(int argc, char *argv[argc]) {
	if (argc != 2) {
		printf("usage: %s <day>", argv[0]);

		return 1;
	}

	const int day = atoi(argv[1]);
	if (!day) {
		printf("Error parsing <day> argument. Must be a number!");
		return 1;
	}
	char *template = read_to_string("template.c");
	if (!template) {
		printf("Error reading template file\n");
		return 1;
	}

	char fname[32];
	int  len = snprintf(fname, sizeof fname, "%02d-1.c", day);
	if (len < 0 || (usize)len >= sizeof fname) {
		printf("Error creating file name for day '%02d'\n", day);
		return 1;
	}
	printf("Creating: %s\n", fname);

	FILE *fp = fopen(fname, "w");
	if (!fp) {
		perror("open target file");
		return 1;
	}

	const char *sep    = "{{DAY}}";
	usize       seplen = strlen(sep);

	char *pref        = template;
	char *placeholder = strstr(template, sep);
	if (!placeholder) {
		printf("Placeholder '%s' not found in template", sep);
		fclose(fp);
		return 1;
	}
	isize preflen = placeholder - pref;
	char *suff    = placeholder + seplen;

	fprintf(fp, "%.*s%02d%s", (int)preflen, pref, day, suff);
	fclose(fp);

	return 0;
}
