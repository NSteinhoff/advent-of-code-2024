#include "prelude.h"

#define DAY "4"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const int expected = 18;
static const char *w = "XMAS";

#define C(X, Y, W) ((Y) * (W + 1) + (X))

static bool search(const char *data, usize x, usize y, usize n, int (*dir)[2]) {
	usize len = strlen(w);
	int dx = (*dir)[0];
	int dy = (*dir)[1];

	for (usize i = 1; i < len; i++) {
		int xx = (int)x + dx * (int)i;
		int yy = (int)y + dy * (int)i;

		if (xx < 0 || xx >= (int)n || yy < 0 || yy >= (int)n)
			return false;

		char c = data[C((usize)xx, (usize)yy, n)];

		if (c != w[i]) return false;
	}

	return true;
}

int solve(char *data) {
	assert(data && "We need data!");
	int result = 0;
	usize n = strcspn(data, "\n");
	printf("%zu x %zu\n", n, n);

	// clang-format off
	int directions[][2] = {
		{ 0, -1},
		{ 1, -1},
		{ 1,  0},
		{ 1,  1},
		{ 0,  1},
		{-1,  1},
		{-1,  0},
		{-1, -1},
	};
	// clang-format on

	// Find the 'X'
	for (usize y = 0; y < n; y++) {
		for (usize x = 0; x < n; x++) {
			if (data[C(x, y, n)] != 'X') continue;

			// Run search in all 8 directions
			for (usize i = 0; i < ASZ(directions); i++) {
				if (search(data, x, y, n, &directions[i]))
					result++;
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
