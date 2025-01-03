#include "prelude.h"

#define DAY    "06"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 41;

#define N 130

static char map[N][N];

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	memset(map, 0, sizeof map);
	usize n = strcspn(data, "\n");
	printf("Map: %zu x %zu\n", n, n);

	int   gx = 0;
	int   gy = 0;
	usize y  = 0;
	foreach_line (data, line) {
		for (usize x = 0; x < n; x++) {
			char c = line[x];
			if (c == '^') {
				gx        = (int)x;
				gy        = (int)y;
				map[y][x] = 'X';
			} else {
				map[y][x] = c;
			}
		}
		y++;
	}

	// clang-format off
	struct {int x, y;} dirs[4] = {
		{ 0, -1}, // N
		{ 1,  0}, // E
		{ 0,  1}, // S
		{-1,  0}, // W
	};
	// clang-format on

	uchar d = 0;
	while (true) {
		map[gy][gx] = 'X';
		int xx      = gx + dirs[d].x;
		int yy      = gy + dirs[d].y;
		if (xx < 0 || yy < 0 || (usize)xx >= n || (usize)yy >= n) break;
		if (map[yy][xx] == '#') {
			d = (d + 1) % CAP(dirs);
		} else {
			gy = yy;
			gx = xx;
		}
	}

	for (usize x = 0; x < n; x++)
		for (usize y = 0; y < n; y++)
			if (map[y][x] == 'X') result++;

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
