// clang-format on
#include "prelude.h"

#define DAY     "16"
#define INPUT   DAY ".txt"
#define SAMPLE  DAY "-s.txt"
#define SAMPLE2 DAY "-s2.txt"

#define N            141
#define M            N
#define MAX_GRID_LEN (M * N)
#define MAX_QUEUE    (MAX_GRID_LEN * 4)
#define D            4

static const i64 expected = 7036;

// Position
typedef struct {
	int x, y;
} P;

// Location
typedef struct {
	P     p;
	usize c, d;
} L;

// Grid
typedef struct {
	usize n, m;
	P     s, e;
	char  tiles[M][N];
	bool  visited[M][N][D];
} G;

// Queue
typedef struct {
	L     elems[MAX_QUEUE];
	usize len;
} Q;

// Adjacencies
typedef struct {
	L     ls[3];
	usize len;
} A;

// Grid
// clang-format off
static inline void visit(G *g, const L *l) { g->visited[l->p.y][l->p.x][l->d] = true; }
static inline bool visited(const G *g, const L *l) { return g->visited[l->p.y][l->p.x][l->d]; }
static inline bool blocked(const G *g, P p) { return g->tiles[p.y][p.x] == '#'; }
static inline L    start(const G *g) { return (L){.p = g->s, .d = 1 /* East */}; }
static inline bool arrived(const G *g, P p) { return p.x == g->e.x && p.y == g->e.y; }
// clang-format on

static void draw(G *g) {
	printf("G: [%3zu, %3zu]\n", g->n, g->m);
	printf("S: [%3d, %3d]\n", g->s.x, g->s.y);
	printf("E: [%3d, %3d]\n", g->e.x, g->e.y);

	bool full = false;
	if (!full) return;

	for (usize y = 0; y < g->m; y++) {
		for (usize x = 0; x < g->n; x++) printf("%c", g->tiles[y][x]);
		printf("\n");
	}
}

// Location
static inline A adj(L l) {
	// clang-format off
	P dp = (P[D]){
		{  0, -1 }, // N
		{  1,  0 }, // E
		{  0,  1 }, // S
		{ -1,  0 }, // W
	}[l.d % D];
	P pp = {.x = l.p.x + dp.x, .y = l.p.y + dp.y};

	return (A){
		.ls = {
			{ .c = l.c +    1, .p =  pp , .d =  l.d          },
			{ .c = l.c + 1000, .p = l.p , .d = (l.d+1  ) % D },
			{ .c = l.c + 1000, .p = l.p , .d = (l.d+D-1) % D },
		},
		.len = 3,
	};
	// clang-format on
}

static inline void show(const L *l) {
	printf("L: %d,%d d=%zu c=%zu\n", l->p.x, l->p.y, l->d, l->c);
}

// Heap
static inline usize parent(size_t i) { return (i - 1) / 2; }
static inline usize left(size_t i) { return i * 2 + 1; }
static inline usize right(size_t i) { return i * 2 + 2; }

static inline bool lower(const Q *q, usize i, usize j) {
	assert(j < q->len && "RHS not in queue");
	return i < q->len && q->elems[i].c < q->elems[j].c;
}

static inline void swap(Q *q, usize i, usize j) {
	L tmp = q->elems[j];
	q->elems[j] = q->elems[i];
	q->elems[i] = tmp;
}

static void heapify_up(Q *q, usize i) {
	for (usize j = parent(i); i && lower(q, i, j); j = parent(i)) {
		swap(q, i, j), i = j;
	}
}

static void heapify_down(Q *q, usize i) {
	assert(i < q->len && "Heapifying nothing!");
	for (usize l = left(i), r = right(i);
	     (lower(q, r, i)) || (lower(q, l, i)); l = left(i), r = right(i)) {
		usize j = lower(q, r, l) ? r : l;
		swap(q, i, j), i = j;
	}
}

static void push(Q *q, L loc) {
	assert(q->len < ASZ(q->elems) && "Queue full!");
	usize i = q->len++;
	q->elems[i] = loc;
	heapify_up(q, i);
}

static L pop(Q *q) {
	assert(q->len && "Queue empty!");
	L head = q->elems[0];
	q->elems[0] = q->elems[--q->len];
	if (q->len) heapify_down(q, 0);
	return head;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	static G g;
	memset(&g, 0, sizeof g);

	// Copy the grid into modifiable memory
	g.n = strcspn(data, "\n");
	g.m = strlen(data) / (g.n + 1);
	for (usize y = 0; y < g.m; y++) {
		for (usize x = 0; x < g.n; x++) {
			char c = data[ATS(x, y, g.n)];
			if (c == 'S') g.s.x = (int)x, g.s.y = (int)y;
			if (c == 'E') g.e.x = (int)x, g.e.y = (int)y;
			g.tiles[y][x] = c == 'S' || c == 'E' ? '.' : c;
		}
	}

	static Q q;
	memset(&q, 0, sizeof q);
	push(&q, start(&g));
	while (q.len) {
		L l = pop(&q);
		if (0) show(&l);
		visit(&g, &l);

		if (arrived(&g, l.p)) {
			result = (i64)l.c;
			break;
		}

		A a = adj(l);
		for (usize i = 0; i < a.len; i++) {
			L ll = a.ls[i];
			if (visited(&g, &ll) || blocked(&g, ll.p)) continue;
			push(&q, ll);
		}
	}

	draw(&g);

	return result;
}

int main(void) {
	const i64 actual = solve(read_to_string(SAMPLE));
	printf("Sample: %lld\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %lld actual %lld\n", expected, actual);
		return 1;
	}
	const i64 actual2 = solve(read_to_string(SAMPLE2));
	const i64 expected2 = 11048;

	printf("Sample2: %lld\n", actual2);
	if (actual2 != expected2) {
		printf("FAIL: expected2 %lld actual2 %lld\n", expected2,
		       actual2);
		return 1;
	}
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
