#include "prelude.h"

#define DAY "1"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

const i64 expected = 31;

#define N 1000

static int compar(const void *x, const void *y) {
	const int *a = x;
	const int *b = y;

	return *a - *b;
}

i64 solve(char *data) {
	int a[N], b[N];
	usize n = 0;

	for_each_line(data, line) {
		int cnt = sscanf(line, "%d %d", &a[n], &b[n]);
		assert(cnt == 2 && "Expect two fields");
		n++;
	}
	printf("N: %zu\n", n);

	qsort(a, n, sizeof *a, compar);
	qsort(b, n, sizeof *b, compar);

	int total = 0;
	usize j = 0;
	for (usize i = 0; i < n; i++) {
		int na = 1;
		int nb = 0;
		// clang-format off
		for (; j     < n && a[i]     >  b[j]; j++) /* pass */; // find match
		for (; j     < n && a[i]     == b[j]; j++) nb++;       // count matches
		for (; i + 1 < n && a[i + 1] == a[i]; i++) na++;       // count instances
		total += a[i] * nb * na;
		// clang-format on
	}

	return total;
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
