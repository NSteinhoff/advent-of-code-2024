// clang-format on
#include "prelude.h"

#define DAY       "23"
#define INPUT     DAY ".txt"
#define SAMPLE    DAY "-s.txt"
#define MAX_NODES (1 << 10)
#define MAX_SETS  (1 << 11)

static const i64 expected = 7;
static const bool verbose = false;

typedef struct {
	const char *nodes[MAX_NODES];
	bool        edges[MAX_NODES][MAX_NODES];
	usize       count;
} Graph;

static usize get(Graph *g, const char *name) {
	for (usize i = 0; i < g->count; i++) {
		if (strcmp(g->nodes[i], name) == 0) return i;
	}

	assert(g->count < CAP(g->nodes));
	usize i     = g->count++;
	g->nodes[i] = name;

	return i;
}

static void print_graph(const Graph *g) {
	printf("Num Nodes: %zu\n", g->count);

	if (!verbose) return;

	printf("   |");
	for (usize i = 0; i < g->count; i++) printf(" %s ", g->nodes[i]);
	printf("\n-- |");
	for (usize i = 0; i < g->count; i++) printf(" -- ");
	printf("\n");
	for (usize i = 0; i < g->count; i++) {
		printf("%s |", g->nodes[i]);
		for (usize j = 0; j < g->count; j++) {
			printf("  %c ", g->edges[i][j] ? 'x' : '.');
		}
		printf("\n");
	}
}

static inline bool eq(const usize a[3], const usize b[3]) {
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

static bool contains(usize len, const usize sets[len][3], const usize s[3]) {
	for (usize i = 0; i < len; i++)
		if (eq(sets[i], s)) return true;
	return false;
}

static int compar(const void *a, const void *b) {
	return (int)*(usize *)a - (int)*(usize *)b;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	/// The graph is an adjacency matrix from every node, to every other node
	static Graph g;
	memset(&g, 0, sizeof g);

	foreach_line (data, line) {
		char *a = line;
		char *b = strchr(line, '-');
		*(b++)  = '\0';

		usize i = get(&g, a);
		usize j = get(&g, b);

		g.edges[i][j] = true;
		g.edges[j][i] = true;
	}

	print_graph(&g);

	static usize sets[MAX_SETS][3];
	usize        num_sets = 0;

	for (usize i = 0; i < g.count; i++) {
		for (usize j = 0; j < g.count; j++) {
			for (usize k = 0; k < g.count; k++) {
				if (i == j || i == k || j == k) continue;

				bool connected = g.edges[i][j] && g.edges[j][k]
				              && g.edges[k][i];
				if (!connected) continue;

				bool has_historian = g.nodes[i][0] == 't'
				                  || g.nodes[j][0] == 't'
				                  || g.nodes[k][0] == 't';
				if (!has_historian) continue;

				usize set[] = {i, j, k};
				qsort(set, CAP(set), sizeof set[0], compar);
				bool seen = contains(num_sets, sets, set);
				if (seen) continue;

				assert(num_sets < MAX_SETS);
				memcpy(sets[num_sets++], set, sizeof set);
			}
		}
	}

	result = (i64)num_sets;

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
