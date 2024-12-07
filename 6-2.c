#include "prelude.h"

#define DAY "6"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const int expected = 6;

#define N 130
// [x,  y]
struct {
	int x, y;
} steps[4] = {
	{0,  -1}, // N
	{1,  0 }, // E
	{0,  1 }, // S
	{-1, 0 }, // W
};

enum {
	EMPTY = 0,
	BLOCKED = 1 << ASZ(steps),
	VISITED = BLOCKED << 1,
};

#define VISIT(x) (x |= VISITED)
#define IS_VISITED(x) (x & VISITED)
#define BLOCK(x) (x |= BLOCKED)
#define IS_BLOCKED(x) (x & BLOCKED)
#define MARK(x, d) (x |= (1 << d))
#define IS_MARKED(x, d) (x & (1 << d))

#define TURN(s) ((s) = (s + 1) % ASZ(steps))
#define IS_OUTSIDE(x, y) (x < 0 || y < 0 || (usize)x >= n || (usize)y >= n)

static u8 map_start[N][N];
static u8 map_block[N][N];
static usize n;


static void visit(u8 m[N][N], uchar d, int x, int y) {
	while (true) {
		if (!IS_VISITED(m[y][x])) VISIT(m[y][x]);

		int xx = x + steps[d].x;
		int yy = y + steps[d].y;
		if (IS_OUTSIDE(xx, yy)) break;
		if (IS_BLOCKED(m[yy][xx])) {
			TURN(d);
			continue;
		}
		y = yy;
		x = xx;
	}
}

static int block(u8 m[N][N], usize bx, usize by, uchar d, int x, int y) {
	BLOCK(m[by][bx]);

	while (true) {
		if (IS_MARKED(m[y][x], d)) return 1;
		MARK(m[y][x], d);

		int xx = x + steps[d].x;
		int yy = y + steps[d].y;
		if (IS_OUTSIDE(xx, yy)) break;
		if (IS_BLOCKED(m[yy][xx])) {
			TURN(d);
			continue;
		}
		y = yy;
		x = xx;
	}

	return 0;
}

int solve(char *data) {
	assert(data && "We need data!");
	int result = 0;

	memset(map_start, 0, sizeof map_start);
	n = strcspn(data, "\n");
	printf("Map: %zu x %zu\n", n, n);

	uchar d = 0;
	int x_start = -1;
	int y_start = -1;

	usize y = 0;
	for_each_line(data, line) {
		for (usize x = 0; x < n; x++) {
			char c = line[x];
			if (c == '^') {
				x_start = (int)x;
				y_start = (int)y;
			} else if (c == '#') {
				BLOCK(map_start[y][x]);
			}
		}
		y++;
	}

	visit(map_start, d, x_start, y_start);

	for (usize x = 0; x < n; x++) {
		for (usize y = 0; y < n; y++) {
			if (!IS_VISITED(map_start[y][x])) continue;
			memcpy(map_block, map_start, sizeof map_block);
			result += block(map_block, x, y, d, x_start, y_start);
		}
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
