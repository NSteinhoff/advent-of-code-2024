#include "prelude.h"

#define DAY    "14"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_BOTS 500

static const i64 expected = 12;
static int       n, m;

typedef struct {
	int x, y;
	int dx, dy;
} R;

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	R     rs[MAX_BOTS];
	usize nr = 0;
	for_each_line(data, line) {
		R r;
		sscanf(line, "p=%d,%d v=%d,%d", &r.x, &r.y, &r.dx, &r.dy);
		printf("%2zu: [%d,%d] -> [%d,%d]\n", nr, r.x, r.y, r.dx, r.dy);
		assert(nr < MAX_BOTS);
		rs[nr++] = r;
	}

	int qs[4][2][2] = {
		{    {0, n / 2},     {0, m / 2}}, // Left-top
		{{n / 2 + 1, n},     {0, m / 2}}, // Right-top
		{    {0, n / 2}, {m / 2 + 1, m}}, // Left-bottom
		{{n / 2 + 1, n}, {m / 2 + 1, m}}, // Right-bottom
	};

	int counts[4] = {0};

	for (usize i = 0; i < nr; i++) {
		R *r   = &rs[i];
		r->x  += n + (r->dx * 100) % n;
		r->y  += m + (r->dy * 100) % m;
		r->x  %= n;
		r->y  %= m;
		int q  = -1;
		for (usize j = 0; q == -1 && j < 4; j++) {
			if (r->x >= qs[j][0][0] && r->x < qs[j][0][1] &&
			    r->y >= qs[j][1][0] && r->y < qs[j][1][1]) {
				counts[j]++;
				q = (int)j;
			}
		}
		printf("Bot: %zu @ [%d,%d] q=%d\n", i, r->x, r->y, q);
	}

	printf("Counts: [%d, %d, %d, %d]\n", counts[0], counts[1], counts[2],
	       counts[3]);

	result = counts[0] * counts[1] * counts[2] * counts[3];

	return result;
}

int main(void) {
	n = 11, m = 7;
	const i64 actual = solve(read_to_string(SAMPLE));
	printf("Sample: %lld\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %lld actual %lld\n", expected, actual);
		return 1;
	}
	n = 101, m = 103;
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
