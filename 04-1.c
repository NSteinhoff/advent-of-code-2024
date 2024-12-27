#include "prelude.h"

#define DAY    "04"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64   expected = 18;
static const char *w        = "XMAS";

static bool search(const char *data, usize x, usize y, usize n, int (*dir)[2]) {
	usize len = strlen(w);
	int   dx  = (*dir)[0];
	int   dy  = (*dir)[1];

	for (usize i = 1; i < len; i++) {
		int xx = (int)x + dx * (int)i;
		int yy = (int)y + dy * (int)i;

		if (xx < 0 || xx >= (int)n || yy < 0 || yy >= (int)n)
			return false;

		char c = data[ATS((usize)xx, (usize)yy, n)];

		if (c != w[i]) return false;
	}

	return true;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64   result = 0;
	usize n      = strcspn(data, "\n");
	printf("%zu x %zu\n", n, n);

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

	// Find the 'X'
	for (usize y = 0; y < n; y++) {
		for (usize x = 0; x < n; x++) {
			if (data[ATS(x, y, n)] != 'X') continue;

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
	const i64 actual = solve(read_to_string(SAMPLE));
	printf("Sample: %lld\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %lld actual %lld\n", expected, actual);
		return 1;
	}
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
