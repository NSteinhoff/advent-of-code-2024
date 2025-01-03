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

	Loc   locations[MAX_ANTENNA][MAX_LOCATION] = {0};
	uchar counts[MAX_ANTENNA]                  = {0};
	uchar freqs[MAX_ANTENNA]                   = {0};
	bool  antinodes[N][N]                      = {0};

	uchar count = 0;

	for (usize y = 0; y < n; y++) {
		for (usize x = 0; x < n; x++) {
			uchar freq = (uchar)data[ATS(x, y, n)];
			if (freq == '.') continue;
			assert(count < MAX_ANTENNA - 1);
			assert(counts[freq] < MAX_LOCATION - 1);
			if (counts[freq] == 1) freqs[count++] = freq;
			locations[freq][counts[freq]++] =
				(Loc){.x = (i8)x, .y = (i8)y};
		}
	}

	for (usize i = 0; i < count; i++) {
		uchar freq = freqs[i];
		for (usize a = 0; a < counts[freq]; a++) {
			Loc *l = &locations[freq][a];
			for (usize b = 0; b < counts[freq]; b++) {
				if (a == b) continue;
				Loc *ll = &locations[freq][b];
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
