// clang-format on
#include "prelude.h"

#define DAY    "18"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define N     71
#define MAX_Q 300

static const i64 expected = 22;

static int size      = 6;
static int num_bytes = 12;

typedef Queue Q;

typedef struct {
	int x, y, s;
} P;

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	bool blocked[N][N] = {0};
	bool visited[N][N] = {0};

	int n = 0;
	for (char *line = strtok(data, "\n"); line; line = strtok(NULL, "\n")) {
		int x, y;
		sscanf(line, "%d,%d", &x, &y);
		blocked[y][x] = true;
		if (++n >= num_bytes) break;
	}

	Q q = {.size = MAX_Q};
	P ps[MAX_Q];

	ps[qpush(&q)] = (P){0};
	while (q.len) {
		P p = ps[qpop(&q)];
		if (visited[p.y][p.x]) continue;
		visited[p.y][p.x] = true;

		if (p.x == size && p.y == size) {
			result = p.s;
			break;
		}

		P adj[4] = {
			{    p.x, p.y - 1, p.s + 1},
			{p.x + 1,     p.y, p.s + 1},
			{    p.x, p.y + 1, p.s + 1},
			{p.x - 1,     p.y, p.s + 1},
		};
		for (usize j = 0; j < 4; j++) {
			P pp = adj[j];
			if (pp.x < 0 || pp.x > size || pp.y < 0 || pp.y > size)
				continue;
			if (blocked[pp.y][pp.x]) continue;
			ps[qpush(&q)] = pp;
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
	size      = 70;
	num_bytes = 1024;
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
