#include "prelude.h"

#define DAY    "12"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 1206;

#define N       150
#define MAX_POS 300
#define MAX_FNC 200

// We can reuse the input string to mark seen tiles by using the 8th bit as a
// flag, because we know that [A-Z] is not using that bit.
// clang-format off
#define CHAR(p) (p.x < 0 || p.y < 0 || p.x >= G.n || p.y >= G.m ? '\0' : (G.d[ATS(p.x, p.y, G.n)]  & (char)~0x80))
#define SEEN(p) (G.d[ATS(p.x, p.y, G.n)] &  (char) 0x80)
#define MARK(p) (G.d[ATS(p.x, p.y, G.n)] |= (char) 0x80)
// clang-format on

/// Position on the grid
typedef struct {
	int x, y;
} Pos;

/// Fence tile
typedef struct {
	Pos   p;
	usize d;
} Fnc;

/// Region with area covered and surrounding fence tiles
typedef struct {
	char  c;
	usize np, nf;
	Pos   ps[MAX_POS];
	Fnc   fs[MAX_FNC];
} Reg;

// clang-format off
static Pos dirs[4] = {
	{ 0, -1}, // N
	{ 1,  0}, // E
	{ 0,  1}, // S
	{-1,  0}, // W
};
// clang-format on

static struct {
	char *d;
	int   n, m;
} G;

static inline Pos step(Pos p, usize d) {
	usize dd = d % CAP(dirs);
	return (Pos){.x = p.x + dirs[dd].x, .y = p.y + dirs[dd].y};
}

static inline bool eq(Fnc lhs, Fnc rhs) {
	return lhs.p.x == rhs.p.x && lhs.p.y == rhs.p.y && lhs.d == rhs.d;
}

static void fill(Reg *r, Pos p, usize d) {
	if (CHAR(p) != r->c) {
		assert(r->nf < MAX_FNC);
		r->fs[r->nf++] = (Fnc){.p = p, .d = d};
		return;
	}
	if (SEEN(p)) return;
	MARK(p);
	assert(r->np < MAX_POS);
	r->ps[r->np++] = p;
	for (usize d = 0; d < CAP(dirs); d++) fill(r, step(p, d), d);
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	G.d = data;
	G.n = (int)strcspn(G.d, "\n");
	G.m = (int)strlen(G.d) / (G.n + 1);
	printf("Grid: %d x %d\n", G.n, G.m);

	for (int y = 0; y < G.m; y++) {
		for (int x = 0; x < G.n; x++) {
			Pos p = {.x = x, .y = y};
			if (SEEN(p)) continue;
			Reg r = {.c = CHAR(p)};
			fill(&r, p, 0);

			// Go around the fence clockwise and count the number of corners.
			// A corner is a fence tile whose neighbor is not a fence tile.
			//
			//   AAAAAf |  move along the fence
			//   AAAAAf |  and count the * corners
			//   AAAAAf |
			//   AAAAAf*v
			//  gfffffg
			//   *
			//   <----
			//
			usize sides = 0;
			for (usize i = 0; i < r.nf; i++) {
				Fnc  f = r.fs[i];
				Fnc  g = {.p = step(f.p, f.d + 1), .d = f.d};
				bool found = false;
				for (usize j = 0; j < r.nf && !found; j++) {
					if (eq(r.fs[j], g)) found = true;
				}
				if (!found) sides++;
			}

			result += r.np * sides;
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
