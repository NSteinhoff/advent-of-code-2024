#include "prelude.h"

#define DAY    "01"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

const i64 expected = 11;

#define N 1000

static int compar(const void *x, const void *y) {
	const int *a = x;
	const int *b = y;

	return *a - *b;
}

i64 solve(char *data) {
	int   xs[N], ys[N];
	usize n = 0;

	foreach_line (data, line) {
		int cnt = sscanf(line, "%d %d", &xs[n], &ys[n]);
		assert(cnt == 2 && "Expect two fields");
		n++;
	}
	printf("N: %zu\n", n);

	qsort(xs, n, sizeof *xs, compar);
	qsort(ys, n, sizeof *ys, compar);

	int total = 0;
	for (usize i = 0; i < n; i++) total += abs(xs[i] - ys[i]);

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
