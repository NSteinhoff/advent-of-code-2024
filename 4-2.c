#include "prelude.h"

#define DAY "4"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const int expected = 9;
const char *w = "MAS";

// clockwise
typedef enum {
	NE,
	SE,
	SW,
	NW,
	NUM_DIRECTIONS,
} Direction;

#define C(X, Y, W) ((Y) * (W + 1) + (X))

static bool search(const char *data, usize x, usize y, usize n, Direction d) {
	printf("Direction: %s\n", d == NE   ? "NE"
	                          : d == SE ? "SE"
	                          : d == SW ? "SW"
	                                    : "NW");

	for (int i = -1; i <= 1; i++) {
		int xx = d == NE || d == SE ? (int)x + (int)i : (int)x - (int)i;
		int yy = d == NE || d == NW ? (int)y - (int)i : (int)y + (int)i;

		if (xx < 0 || xx >= (int)n || yy < 0 || yy >= (int)n)
			return false;

		char c = data[C((usize)xx, (usize)yy, n)];
		printf("%d,%d = %c\n", xx, yy, c);
		if (c != w[i+1]) {
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
			if (data[C(x, y, n)] != 'A') continue;
			printf("Search @ %zu,%zu\n", x, y);

			// Run search in all 4 X directions
			usize num = 0;
			for (Direction d = NE; d < NUM_DIRECTIONS; d++) {
				if (search(data, x, y, n, d)) num++;
			}
			if (num == 2) result++;
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
