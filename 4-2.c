#include "prelude.h"

#define DAY "4"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 9;
const char *w = "MAS";

static bool search(const char *data, usize x, usize y, usize n, int (*dir)[2]) {
	int dx = (*dir)[0];
	int dy = (*dir)[1];

	for (int i = -1; i <= 1; i++) {
		int xx = (int)x + dx * (int)i;
		int yy = (int)y + dy * (int)i;
		char c = data[ATS((usize)xx, (usize)yy, n)];
		if (c != w[i + 1]) return false;
	}

	return true;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;
	usize n = strcspn(data, "\n");
	printf("%zu x %zu\n", n, n);

	// clang-format off
	int directions[][2] = {
		{ 1, -1},
		{ 1,  1},
		{-1,  1},
		{-1, -1},
	};
	// clang-format on

	// Find the 'A'
	for (usize y = 1; y < n - 1; y++) {
		for (usize x = 1; x < n - 1; x++) {
			if (data[ATS(x, y, n)] != 'A') continue;

			// Run search in the 4 diagonals
			usize num = 0;
			for (usize i = 0; i < ASZ(directions); i++) {
				if (search(data, x, y, n, &directions[i]))
					num++;
			}
			if (num == 2) result++;
		}
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
