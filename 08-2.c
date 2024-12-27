#include "prelude.h"

#define DAY    "08"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define N            50
#define MAX_ANTENNA  256
#define MAX_LOCATION 32

static const i64 expected = 34;

typedef struct {
	i8 x, y;
} Loc;

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	usize n = strcspn(data, "\n");
	assert(n <= N);

	Loc   locs[MAX_ANTENNA][MAX_LOCATION] = {0};
	uchar cs[MAX_ANTENNA]                 = {0};
	uchar ks[MAX_ANTENNA]                 = {0};
	bool  antinodes[N][N]                 = {0};

	uchar k = 0;

	for (usize y = 0; y < n; y++) {
		for (usize x = 0; x < n; x++) {
			uchar c = (uchar)data[ATS(x, y, n)];
			if (c == '.') continue;
			assert(k < MAX_ANTENNA - 1);
			assert(ks[c] < MAX_LOCATION - 1);
			locs[c][ks[c]++] = (Loc){.x = (i8)x, .y = (i8)y};
			if (ks[c] == 1) cs[k++] = c;
		}
	}

	for (usize i = 0; i < k; i++) {
		uchar c = cs[i];
		for (usize u = 0; u < ks[c]; u++) {
			Loc *l = &locs[c][u];
			for (usize w = 0; w < ks[c]; w++) {
				if (w == u) continue;
				Loc *ll = &locs[c][w];
				i8   dx = l->x - ll->x;
				i8   dy = l->y - ll->y;
				for (i8 x = l->x, y = l->y;
				     x >= 0 && y >= 0 && x < (i8)n && y < (i8)n;
				     x += dx, y += dy)
					antinodes[y][x] = true;
			}
		}
	}

	for (usize x = 0; x < n; x++)
		for (usize y = 0; y < n; y++) result += antinodes[y][x];

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
