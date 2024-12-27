// clang-format on
#include "prelude.h"

#define DAY    "20"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_Q 4
#define M     141
#define N     M

static int       saving   = 20;
static const i64 expected = 5;

typedef struct {
	int x, y;
} P;

static struct {
	int m, n;
	P   s, e;
} G;

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	G.n = (int)strcspn(data, "\n");
	G.m = (int)strlen(data) / (G.n + 1);
	printf("G: %d x %d\n", G.m, G.n);
	assert(G.n <= N && G.m <= M);

	int  dists[M][N]   = {0};
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

	P p             = G.s;
	dists[p.y][p.x] = 0;
	while (p.x != G.e.x || p.y != G.e.y) {
		P adj[] = {
			{    p.x, p.y - 1},
			{p.x + 1,     p.y},
			{    p.x, p.y + 1},
			{p.x - 1,     p.y},
		};

		for (usize j = 0; j < ASZ(adj); j++) {
			P pp = adj[j];
			if (pp.x < 0 || pp.x >= G.n || pp.y < 0 || pp.y >= G.m)
				continue;
			if (blocked[pp.y][pp.x]) continue;
			if (dists[pp.y][pp.x] != -1) continue;
			dists[pp.y][pp.x] = dists[p.y][p.x] + 1;
			p                 = pp;
		}
	}

	for (int y = 1; y < G.m - 1; y++) {
		for (int x = 1; x < G.n - 1; x++) {
			if (blocked[y][x]) continue;

			P cheats[] = {
				{    x, y - 2},
                                {x + 2,     y},
                                {    x, y + 2},
				{x - 2,     y},
                                {x + 1, y - 1},
                                {x + 1, y + 1},
				{x - 1, y + 1},
                                {x - 1, y - 1},
			};

			for (usize j = 0; j < ASZ(cheats); j++) {
				P pp = cheats[j];
				if (pp.x < 0 || pp.x >= G.n || pp.y < 0 ||
				    pp.y >= G.m)
					continue;
				if (blocked[pp.y][pp.x]) continue;
				int saved = dists[y][x] - dists[pp.y][pp.x] - 2;
				if (saved >= saving) result++;
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
	saving = 100;
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
