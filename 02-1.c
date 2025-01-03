#include "prelude.h"

#define DAY    "02"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

const i64 expected = 2;

#define N 16

static bool safe(usize size, int xs[size]) {
	for (usize i = 1; i < size; i++) {
		int diff = abs(xs[i] - xs[i - 1]);
		if (diff < 1 || diff > 3) return false;
		if (i >= 2 && (xs[i] - xs[i - 1]) * (xs[i - 1] - xs[i - 2]) < 0)
			return false;
	}

	return true;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	foreach_line (data, line) {
		usize n     = 0;
		int   xs[N] = {0};
		foreach_word (line, v) xs[n++] = atoi(v);
		if (safe(n, xs)) result++;
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
