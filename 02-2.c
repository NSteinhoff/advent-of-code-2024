#include "prelude.h"

#define DAY "02"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

const i64 expected = 4;

#define MAX_VALUES 16

static bool safe(usize size, int xs[size]) {
	for (usize i = 1; i < size; i++) {
		int diff = abs(xs[i] - xs[i - 1]);
		if (diff < 1 || diff > 3) return false;
		if (i >= 2 && (xs[i] - xs[i - 1]) * (xs[i - 1] - xs[i - 2]) < 0)
			return false;
	}

	return true;
}

static bool safeskip(usize size, int xs[size], usize skip) {
	int ys[MAX_VALUES] = {0};

	usize n = 0;
	for (usize i = 0; i < size; i++)
		if (i != skip) ys[n++] = xs[i];

	return safe(n, ys);
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;
	for_each_line(data, line) {
		printf("%s ", line);
		usize n = 0;
		int xs[MAX_VALUES] = {0};
		for_each_word(line, v) xs[n++] = atoi(v);
		bool is_safe = safe(n, xs);
		for (usize i = 0; i < n && !is_safe; i++)
			is_safe = safeskip(n, xs, i);

		if (is_safe) result++;
		printf("%s\n", is_safe ? "SAFE" : "UNSAFE");
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
