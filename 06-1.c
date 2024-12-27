#include "prelude.h"

#define DAY "06"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 41;

#define N 130

static char map[N][N];

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;
	memset(map, 0, sizeof map);
	usize len = strcspn(data, "\n");
	printf("Map: %zu x %zu\n", len, len);

	int gx = 0;
	int gy = 0;
	usize y = 0;
	for_each_line(data, line) {
		for (usize x = 0; x < len; x++) {
			char c = line[x];
			if (c == '^') {
				gx = (int)x;
				gy = (int)y;
				map[y][x] = 'X';
			} else {
				map[y][x] = c;
			}
		}
		y++;
	}

	struct {
		int x, y;
	} ds[4] = {
		{0,  -1}, // N
		{1,  0 }, // E
		{0,  1 }, // S
		{-1, 0 }, // W
	};

	uchar d = 0;
	while (true) {
		map[gy][gx] = 'X';
		int xx = gx + ds[d].x;
		int yy = gy + ds[d].y;
		if (xx < 0 || yy < 0 || (usize)xx >= len || (usize)yy >= len) break;
		if (map[yy][xx] == '#') {
			d = (d + 1) % ASZ(ds);
		} else {
			gy = yy;
			gx = xx;
		}
	}

	for (usize x = 0; x < len; x++)
		for (usize y = 0; y < len; y++)
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
