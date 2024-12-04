#include "prelude.h"

#define DAY "4"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const int expected = 18;
static const char *w = "XMAS";

// clockwise
typedef enum {
	N,
	E,
	S,
	W,
	NE,
	SE,
	SW,
	NW,
	NUM_DIRECTIONS,
} Direction;

#define C(X, Y, W) ((Y) * (W + 1) + (X))

static bool search(const char *data, usize x, usize y, usize n, Direction d) {
	usize len = strlen(w);

	printf("Direction: %s\n", d == N    ? "N"
	                          : d == NE ? "NE"
	                          : d == E  ? "E"
	                          : d == SE ? "SE"
	                          : d == S  ? "S"
	                          : d == SW ? "SW"
	                          : d == W  ? "W"
	                                    : "NW");

	for (usize i = 1; i < len; i++) {
		int xx = d == N || d == S             ? (int)x
		       : d == NE || d == E || d == SE ? (int)x + (int)i
		                                      : (int)x - (int)i;
		int yy = d == E || d == W             ? (int)y
		       : d == N || d == NE || d == NW ? (int)y - (int)i
		                                      : (int)y + (int)i;

		if (xx < 0 || xx >= (int)n || yy < 0 || yy >= (int)n)
			return false;

		char c = data[C((usize)xx, (usize)yy, n)];
		printf("%d,%d = %c\n", xx, yy, c);
		if (c != w[i]) {
			return false;
		}
	}

	return true;
}

int solve(char *data) {
	assert(data && "We need data!");
	int result = 0;
	usize n = strcspn(data, "\n");
	printf("%zu x %zu\n", n, n);

	// Find the 'X'
	for (usize y = 0; y < n; y++) {
		for (usize x = 0; x < n; x++) {
			if (data[C(x, y, n)] != 'X') continue;
			printf("Search @ %zu,%zu\n", x, y);

			// Run search in all 8 directions
			for (Direction d = N; d < NUM_DIRECTIONS; d++) {
				if (search(data, x, y, n, d)) result++;
			}
		}
	}

	return result;
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
