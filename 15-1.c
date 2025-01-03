#include "prelude.h"

#define DAY    "15"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 10092;

typedef struct {
	int x, y;
} P;

static struct {
	char *d;
	int   n, m;
} G;

static P dirs[] = {
	['^'] = {0,  -1},
	['>'] = {1,  0 },
	['v'] = {0,  1 },
	['<'] = {-1, 0 },
};

static inline char get(P p) {
	return G.d[ATS((usize)p.x, (usize)p.y, (usize)G.n)];
}

static inline void set(P p, char c) {
	G.d[ATS((usize)p.x, (usize)p.y, (usize)G.n)] = c;
}

static inline P step(P p, P s) { return (P){.x = p.x + s.x, .y = p.y + s.y}; }

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	char *grid   = data;
	char *moves  = strstr(data, "\n\n");
	// Split at the second NL to keep the traling NL in the last row
	moves[1]     = '\0';
	moves       += 2;

	G.d = grid;
	G.n = (int)strcspn(G.d, "\n");
	G.m = (int)strlen(G.d) / (G.n + 1);

	P    p     = {0};
	bool found = false;
	for (int y = 0; y < G.m && !found; y++) {
		for (int x = 0; x < G.n && !found; x++) {
			if (get(p = (P){x, y}) == '@') found = true;
		}
	}
	set(p, '.');

	for (char *mv = moves; *mv; mv++) {
		if (*mv == '\n') continue;
		P d  = dirs[(usize)*mv];
		P pp = step(p, d);
		for (; get(pp) == 'O'; pp = step(pp, d));
		if (get(pp) == '#') continue;
		set(pp, 'O');
		p = step(p, d);
		set(p, '.');
	}

	for (int y = 0; y < G.m; y++) {
		for (int x = 0; x < G.n; x++) {
			if (get((P){x, y}) == 'O') {
				result += 100 * y + x;
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
