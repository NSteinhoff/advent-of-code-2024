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
#define CHAR(p) (p.x < 0 || p.y < 0 || p.x >= G.n || p.y >= G.m ? 0 : (G.d[ATS(p.x, p.y, G.n)]  & (char)~0x80))
#define SEEN(p) (G.d[ATS(p.x, p.y, G.n)]  & (char) 0x80)
#define MARK(p) (G.d[ATS(p.x, p.y, G.n)] |= (char) 0x80)
// clang-format on

typedef struct {
	int x, y;
} P;

typedef struct {
	P     p;
	usize d;
} F;

typedef struct {
	char  c;
	usize np, nf;
	P     ps[MAX_POS];
	F     fs[MAX_FNC];
} R;

static P dirs[4] = {
	{ 0, -1}, // N
	{ 1,  0}, // E
	{ 0,  1}, // S
	{-1,  0}, // W
};

static struct {
	char *d;
	int   n, m;
} G;

static inline P step(P p, usize d) {
	usize dd = d % ASZ(dirs);
	return (P){.x = p.x + dirs[dd].x, .y = p.y + dirs[dd].y};
}

static inline bool eq(F lhs, F rhs) {
	return lhs.p.x == rhs.p.x && lhs.p.y == rhs.p.y && lhs.d == rhs.d;
}

static void fill(R *r, P p, usize d) {
	if (SEEN(p) && CHAR(p) == r->c) return;
	if (CHAR(p) != r->c) {
		assert(r->nf < MAX_FNC);
		r->fs[r->nf++] = (F){.p = p, .d = d};
		return;
	}
	MARK(p);
	assert(r->np < MAX_POS);
	r->ps[r->np++] = p;
	for (usize d = 0; d < ASZ(dirs); d++) fill(r, step(p, d), d);
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
			P p = {x, y};
			if (SEEN(p)) continue;
			R r = {.c = CHAR(p)};
			fill(&r, p, 0);

			usize ns = 0;
			for (usize i = 0; i < r.nf; i++) {
				F    f = r.fs[i];
				F    g = (F){.p = step(f.p, f.d + 1), .d = f.d};
				bool found = false;
				for (usize j = 0; j < r.nf && !found; j++) {
					if (eq(r.fs[j], g)) found = true;
				}
				if (!found) ns++;
			}

			result += r.np * ns;
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
