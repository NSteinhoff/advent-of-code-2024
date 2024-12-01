#include "prelude.h"

#define DAY "1"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

const int expected = 11;

#define N 1000

static int compar(const void *x, const void *y) {
	const int *a = x;
	const int *b = y;

	return *a - *b;
}

int solve(char *data) {
	int a[N], b[N];
	usize n = 0;

	for (const char *s = data; n < N; n++) {
		int cnt = sscanf(s, "%d %d", &a[n], &b[n]);
		if (cnt != 2) break;
		s = strchr(s, '\n');
		if (!s) break;
		s++;
	}
	printf("N: %zu\n", n);

	qsort(a, n, sizeof *a, compar);
	qsort(b, n, sizeof *b, compar);

	int total = 0;
	for (usize i = 0; i < n; i++) {
		total += abs(a[i] - b[i]);
	}

	return total;
}

int main(void) {
	const int actual = solve(read_to_string(SAMPLE));
	printf("Example: %d\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %d actual %d\n", expected, actual);
		return 1;
	}
	printf("Result: %d\n", solve(read_to_string(INPUT)));
	return 0;
}
