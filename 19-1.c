// clang-format on
#include "prelude.h"

#define DAY    "19"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_PATTERNS 500
#define MAX_DESIGNS  400
#define MAX_STACK    64

static const i64 expected = 6;

static inline char *split(char *s, const char *sep) {
	if (!sep[0]) return s;
	s = strstr(s, sep);
	if (!s) return NULL;
	s[0] = '\0';
	s += strlen(sep);
	return s;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64   result = 0;
	char *patterns = data;
	char *designs = split(data, "\n\n");

	char *ps[MAX_PATTERNS], *ds[MAX_DESIGNS];
	usize np = 0, nd = 0;

	for_each_token(patterns, p, ", ") {
		assert(np < MAX_PATTERNS);
		ps[np++] = p;
	}
	for_each_token(designs, d, "\n") {
		assert(nd < MAX_DESIGNS);
		ds[nd++] = d;
	}

	printf("Patterns: %zu\n", np);
	printf("Designs: %zu\n", nd);

	usize count = 0;
	for (usize i = 0; i < nd; i++) {
		char *stack[MAX_STACK];
		usize top = 0;
		stack[top++] = ds[i];
		while (top--) {
			char *d = stack[top];
			if (!*d) {
				count++;
				break;
			}
			for (usize j = 0; j < np; j++) {
				char *p = ps[j];
				usize len = strlen(p);
				if (!strncmp(d, p, len)) {
					assert(top < MAX_STACK);
					stack[top++] = d + len;
				}
			}
		}
	}
	result = (i64)count;

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
