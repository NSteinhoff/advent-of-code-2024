#include "prelude.h"

#define DAY    "15"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 9021;

#define N 100
#define M 50

#define MAX_MOVING 100 * 50

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

static inline usize push(usize len, P *moving, P p) {
	for (usize i = 0; i < len; i++)
		if (p.x == moving[i].x && p.y == moving[i].y) return len;
	assert(len < MAX_MOVING);
	moving[len++] = p;
	return len;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	char  grid[N * M + M + 1];
	char *moves  = strstr(data, "\n\n");
	// Split at the second NL to keep the traling NL in the last row
	moves[1]     = '\0';
	moves       += 2;
	usize len    = 0;
	for (char *c = data; *c; c++) {
		if (*c == 'O') {
			grid[len++] = '[';
			grid[len++] = ']';
		} else if (*c == '@') {
			grid[len++] = '@';
			grid[len++] = '.';
		} else if (*c == '\n') {
			grid[len++] = '\n';
		} else {
			grid[len++] = *c;
			grid[len++] = *c;
		}
	}
	grid[len] = '\0';

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

	for (char *mv = moves; *mv; mv++) {
		if (*mv == '\n') continue;
		P d = dirs[(usize)*mv];

		P     moving[MAX_MOVING];
		usize len = push(0, moving, p);
		bool  go  = true;
		for (usize i = 0; i < len && go; i++) {
			P p  = moving[i];
			P pp = step(p, d);
			if (get(pp) == '#') {
				go = false;
				break;
			} else if (get(pp) == '[') {
				len = push(len, moving, pp);
				if (d.x == 0)
					len =
						push(len, moving,
					             (P){pp.x + 1, pp.y});
			} else if (get(pp) == ']') {
				len = push(len, moving, pp);
				if (d.x == 0)
					len =
						push(len, moving,
					             (P){pp.x - 1, pp.y});
			}
		}
		if (go) {
			while (len--) {
				P p  = moving[len];
				P pp = step(p, d);
				assert(get(pp) == '.');
				set(pp, get(p));
				set(p, '.');
			}
			p = step(p, d);
		}
	}

	for (int y = 0; y < G.m; y++) {
		for (int x = 0; x < G.n; x++) {
			if (get((P){x, y}) == '[') {
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
