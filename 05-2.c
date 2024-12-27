#include "prelude.h"

#define DAY    "05"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define N 100

static const i64 expected = 123;

static int   rules[N][N] = {0};
static usize num[N]      = {0};

static int cmp(const void *left, const void *right) {
	int a = *(int *)left;
	int b = *(int *)right;

	for (usize i = 0; i < num[a]; i++)
		if (rules[a][i] == b) return -1;
	for (usize i = 0; i < num[b]; i++)
		if (rules[b][i] == a) return 1;

	return 0;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;
	memset(rules, 0, sizeof rules);
	memset(num, 0, sizeof num);

	char *updates  = strstr(data, "\n\n");
	*(updates++)   = '\0';
	updates       += strspn(updates, "\n");

	for_each_line(data, line) {
		int a, b;
		sscanf(line, "%u|%u", &a, &b);
		rules[a][num[a]++] = b;
	}

	for_each_line(updates, line) {
		int   pages[N] = {0};
		usize len      = 0;

		for_each_token(line, s, ",") pages[len++] = atoi(s);

		bool valid = true;
		for (usize i = 0; i < len && valid; i++) {
			int a = pages[i];
			for (usize j = 0; j < i && valid; j++) {
				int b = pages[j];
				for (usize k = 0; k < num[a] && valid; k++) {
					if (b == rules[a][k]) valid = false;
				}
			}
		}
		if (!valid) {
			qsort(pages, len, sizeof *pages, cmp);
			result += pages[len / 2];
		}
	}

	return result;
}

int main(void) {
	const i64 actual = solve(read_to_string(SAMPLE));
	printf("Sample: %lld\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %lld actual %lld\n", expected, actual);
		return 1;
	}
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
