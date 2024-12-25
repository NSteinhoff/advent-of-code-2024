// clang-format on
#include "prelude.h"

#define DAY    "20"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_Q 4
#define M     141
#define N     M

static int       saving = 72;
static const i64 expected = 29;

typedef struct {
	int x, y;
} P;

static struct {
	int m, n;
	P   s, e;
} G;

static inline bool contains(usize n, P ps[n], P p) {
	for (usize i = 0; i < n; i++) {
		P pp = ps[i];
		if (p.x == pp.x && p.y == pp.y) return true;
	}
	return false;
}

static inline bool outside(P p) {
	return p.x < 0 || p.x >= G.n || p.y < 0 || p.y >= G.m;
}

static uint count(P p, const int dists[M][N]) {
	const int radius = 20;
	uint      count = 0;
	for (int r = 2; r <= radius; r++) {
		for (int dy = 0; dy <= r; dy++) {
			int dx = r - dy;
			P   cheats[] = {
                                {p.x + dx, p.y + dy},
                                {p.x + dx, p.y - dy},
                                {p.x - dx, p.y + dy},
                                {p.x - dx, p.y - dy},
                        };
			for (usize i = 0; i < ASZ(cheats); i++) {
				P pp = cheats[i];
				if (outside(pp)) continue;
				if (dists[pp.y][pp.x] == -1) continue;
				if (contains(i, cheats, pp)) continue;
				int saved =
					dists[p.y][p.x] - dists[pp.y][pp.x] - r;
				if (saved >= saving) count++;
			}
		}
	}
	return count;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;
	G.n = (int)strcspn(data, "\n");
	G.m = (int)strlen(data) / (G.n + 1);
	printf("G: %d x %d\n", G.m, G.n);
	assert(G.n <= N && G.m <= M);

	int  dists[M][N] = {0};
	bool blocked[M][N] = {0};

	for (int y = 0; y < G.m; y++) {
		for (int x = 0; x < G.n; x++) {
			char c = data[ATS(x, y, G.n)];
			if (c == '#') blocked[y][x] = true;
			if (c == 'S') G.s = (P){.x = x, .y = y};
			if (c == 'E') G.e = (P){.x = x, .y = y};
			dists[y][x] = -1;
		}
	}

	printf("S: %d,%d | E:%d,%d\n", G.s.x, G.s.y, G.e.x, G.e.y);

	P p = G.s;
	dists[p.y][p.x] = 0;
	while (p.x != G.e.x || p.y != G.e.y) {
		// clang-format off
		P adj[] = {
			{p.x    , p.y - 1},
			{p.x + 1, p.y    },
			{p.x    , p.y + 1},
			{p.x - 1, p.y    },
		};
		// clang-format on

		for (usize j = 0; j < ASZ(adj); j++) {
			P pp = adj[j];
			if (outside(pp)) continue;
			if (blocked[pp.y][pp.x]) continue;
			if (dists[pp.y][pp.x] != -1) continue;
			dists[pp.y][pp.x] = dists[p.y][p.x] + 1;
			p = pp;
		}
	}

	for (int y = 1; y < G.m - 1; y++) {
		for (int x = 1; x < G.n - 1; x++) {
			if (blocked[y][x]) continue;
			result += count((P){x, y}, dists);
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
	saving = 100;
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
