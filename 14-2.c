#include "prelude.h"

#define DAY    "14"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_BOTS 500

static const i64 expected = 1;
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
		assert(nr < MAX_BOTS);
		rs[nr++] = r;
	}

	for (usize i = 0; i < (usize)(n * m); i++) {
		for (usize j = 0; j < nr; j++) {
			R *r = &rs[j];
			r->x = (r->x + n + r->dx) % n;
			r->y = (r->y + m + r->dy) % m;
		}
		bool overlap = false;
		for (usize j = 0; !overlap && j < nr; j++) {
			for (usize k = 0; !overlap && k < nr; k++) {
				if (j == k) continue;
				if (rs[j].x == rs[k].x && rs[j].y == rs[k].y)
					overlap = true;
			}
		}
		if (!overlap) {
			result = (i64)i + 1;
			break;
		}
	}

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
