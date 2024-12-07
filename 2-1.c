#include "prelude.h"

#define DAY "2"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

const int expected = 2;

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

int solve(char *data) {
	assert(data && "We need data!");
	int result = 0;
	for_each_line(data, line) {
		printf("%s ", line);
		usize n = 0;
		int xs[MAX_VALUES] = {0};
		for_each_word(line, v) xs[n++] = atoi(v);
		bool is_safe = safe(n, xs);
		if (is_safe) result++;
		printf("%s\n", is_safe ? "SAFE" : "UNSAFE");
	}

	return result;
}

int main(void) {
	const int actual = solve(read_to_string(SAMPLE));
	printf("Sample: %d\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %d actual %d\n", expected, actual);
		return 1;
	}
	printf("Result: %d\n", solve(read_to_string(INPUT)));
	return 0;
}
