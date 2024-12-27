// clang-format on
#include "prelude.h"

#define DAY    "18"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_BYTES 4000
#define N         71
#define MAX_Q     300

static const i64 expected = 0;

static int size      = 6;
static int num_bytes = 12;

typedef struct {
	int x, y;
} P;

typedef struct {
	usize s, e, size, len;
} Q;

static usize push(Q *q) {
	assert(q->len < q->size);
	usize i = q->e;
	q->e    = q->e < q->size - 1 ? q->e + 1 : 0;
	q->len++;
	return i;
}

static usize pop(Q *q) {
	assert(q->len > 0);
	usize i = q->s;
	q->s    = q->s < q->size - 1 ? q->s + 1 : 0;
	q->len--;
	return i;
}

static bool connected(usize len, int drops[len][2], usize n) {
	bool blocked[N][N] = {0};
	bool visited[N][N] = {0};

	for (usize i = 0; i < n; i++) blocked[drops[i][0]][drops[i][1]] = true;

	Q q = {.size = MAX_Q};
	P ps[MAX_Q];

	ps[push(&q)] = (P){0};
	while (q.len) {
		P p = ps[pop(&q)];
		if (p.x == size && p.y == size) return true;

		if (visited[p.y][p.x]) continue;
		visited[p.y][p.x] = true;

		P adj[4] = {
			{    p.x, p.y - 1},
			{p.x + 1,     p.y},
			{    p.x, p.y + 1},
			{p.x - 1,     p.y},
		};
		for (usize j = 0; j < 4; j++) {
			P pp = adj[j];
			if (pp.x < 0 || pp.x > size || pp.y < 0 || pp.y > size)
				continue;
			if (blocked[pp.y][pp.x]) continue;
			ps[push(&q)] = pp;
		}
	}

	return false;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	int   drops[MAX_BYTES][2] = {0};
	usize len                 = 0;

	for (char *line = strtok(data, "\n"); line; line = strtok(NULL, "\n")) {
		int x, y;
		sscanf(line, "%d,%d", &x, &y);
		drops[len][0] = x;
		drops[len][1] = y;
		len++;
	}

	usize lo = 0;   // connected
	usize hi = len; // disconnected
	while (hi - lo > 1) {
		usize mid = lo + (hi - lo) / 2;
		if (connected(len, drops, mid))
			lo = mid;
		else
			hi = mid;
	}
	assert(lo == hi - 1);
	usize i = hi - 1;
	printf("Bad drop @ %zu\t[%d,%d]\n", hi, drops[i][0], drops[i][1]);

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
