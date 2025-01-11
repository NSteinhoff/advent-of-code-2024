// clang-format on
#include "prelude.h"

#define DAY       "23"
#define INPUT     DAY ".txt"
#define SAMPLE    DAY "-s.txt"
#define MAX_NODES (1 << 10)
#define MAX_CLIQS (1 << 19)
#define MAX_MEMBS 16

static const i64  expected = 0;
static const bool verbose  = false;

typedef struct {
	const char *nodes[MAX_NODES];
	bool        edges[MAX_NODES][MAX_NODES];
	u16         count;
} Graph;

typedef struct {
	u16 members[MAX_MEMBS];
	u8  count;
} Clq;

typedef struct {
	Clq   elements[MAX_CLIQS];
	usize cap;
	usize count;
	/// Index of maximum clique
	usize max_idx;
	/// Number of lookups
	usize lookups;
	/// Number of probes
	usize probes;
} Clqs;

static u16 node_idx(Graph *g, const char *name) {
	for (u16 i = 0; i < g->count; i++)
		if (strcmp(g->nodes[i], name) == 0) return i;

	assert(g->count < CAP(g->nodes));
	assert(g->count < UINT16_MAX);
	g->nodes[g->count] = name;

	return g->count++;
}

static void print_graph(const Graph *g) {
	printf("Num Nodes: %u\n", g->count);

	if (!verbose) return;

	printf("   |");
	for (u16 i = 0; i < g->count; i++) printf(" %s ", g->nodes[i]);
	printf("\n-- |");
	for (u16 i = 0; i < g->count; i++) printf(" -- ");
	printf("\n");
	for (u16 i = 0; i < g->count; i++) {
		printf("%s |", g->nodes[i]);
		for (u16 j = 0; j < g->count; j++) {
			printf("  %c ", g->edges[i][j] ? 'x' : '.');
		}
		printf("\n");
	}
}

static int cmp_str(const void *a, const void *b) {
	return strcmp(*(const char **)a, *(const char **)b);
}

static void print_clq(const Graph *g, const Clq *clq) {
	const char *names[CAP(clq->members)];
	for (u8 i = 0; i < clq->count; i++)
		names[i] = g->nodes[clq->members[i]];
	qsort(names, clq->count, sizeof names[0], cmp_str);

	printf("(%u)[", clq->count);
	for (u8 i = 0; i < clq->count; i++)
		printf("%s%s", i ? "," : "", names[i]);
	printf("]\n");
}

static void print_clqs(const Graph *g, const Clqs *clqs) {
	printf("Cliques: %zu\t", clqs->count);
	printf("Lookups: %zu\t", clqs->lookups);
	printf("Probes/Lookup: %.2f\n", (f64)clqs->probes / (f64)clqs->lookups);
	printf("Load Factor: %.2f\n", (f64)clqs->count / ((f64)clqs->cap));
	printf("Memory Usage: %.2f MB\n", (f64)sizeof *clqs / (f64)(1 << 20));

	if (!verbose) return;
	for (usize i = 0; i < clqs->count; i++)
		print_clq(g, clqs->elements + i);
}

static bool contains(const Clq *clq, u16 node) {
	for (u8 i = 0; i < clq->count; i++) {
		if (clq->members[i] == node) return true;
	}

	return false;
}

static bool connected(const Graph *g, const Clq *clq, u16 node) {
	for (u8 i = 0; i < clq->count; i++) {
		if (!g->edges[node][clq->members[i]]) return false;
	}

	return true;
}

static int cmp_node(const void *a, const void *b) {
	u16 n1 = *(u16 *)a;
	u16 n2 = *(u16 *)b;
	return n1 > n2 ? 1 : n1 < n2 ? -1 : 0;
}

static void sort(Clq *clq) {
	qsort(clq->members, clq->count, sizeof clq->members[0], cmp_node);
}

/// Check equality via memcmp.
/// This works because the members array is always sorted.
static bool eq(const Clq *left, const Clq *right) {
	if (left->count != right->count) return false;
	return !memcmp(
		left->members, right->members,
		sizeof left->members[0] * left->count);
}

/// Create a clone with the new member added.
/// Ensure that the members array is sorted.
static Clq with_member_added(Clq clq, u16 node) {
	assert(clq.count < CAP(clq.members));
	assert(clq.count < UINT8_MAX);
	clq.members[clq.count++] = node;
	sort(&clq);
	return clq;
}

/// Hash of clque count and elements
static usize hash(const Clq *clq) {
	u32 h = 5381;

	h = ((h << 5) + h) + (u8)clq->count;
	for (u8 i = 0; i < clq->count; i++) {
		u16 node = clq->members[i];
		h        = ((h << 5) + h) + (u8)(node >> 8);
		h        = ((h << 5) + h) + (u8)(node & 0xff);
	}
	return h;
}

/// Add a clque to the set of known clques.
/// Returns 'true' if the new clque was added, 'false' if it already existed
/// in the set.
static bool add(Clqs *clqs, const Clq *clq) {
	clqs->lookups++;
	usize start = hash(clq) % clqs->cap;

	for (usize i = 0; i < clqs->cap; i++) {
		clqs->probes++;
		usize idx = (start + i) % clqs->cap;

		// Fill into empty slot
		if (clqs->elements[idx].count == 0) {
			clqs->elements[idx] = *clq;
			clqs->count++;

			// Keep track of the maximum
			if (clq->count > clqs->elements[clqs->max_idx].count)
				clqs->max_idx = idx;

			return true;
		};

		// Skip if exists
		if (eq(&clqs->elements[idx], clq)) {
			return false;
		}
	}

	UNREACHABLE("SET FULL!");
}

/// Search starting from 'node' trying to complete 'clq'.
static void search(const Graph *g, u16 node, const Clq *clq, Clqs *clqs) {
	Clq  next = with_member_added(*clq, node);
	bool seen = !add(clqs, &next);
	if (seen) return;

	for (u16 i = 0; i < g->count; i++) {
		if (contains(&next, i)) continue;
		if (!connected(g, &next, i)) continue;
		search(g, i, &next, clqs);
	}
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

		u16 i = node_idx(&g, a);
		u16 j = node_idx(&g, b);

		g.edges[i][j] = true;
		g.edges[j][i] = true;
	}

	print_graph(&g);

	static Clqs clqs;
	memset(&clqs, 0, sizeof clqs);
	clqs.cap = CAP(clqs.elements);

	Clq clq = {0}; // Start from an empty clque
	for (u16 i = 0; i < g.count; i++) search(&g, i, &clq, &clqs);
	print_clqs(&g, &clqs);

	printf("MAXIMUM: ");
	Clq *max_clq = &clqs.elements[clqs.max_idx];
	print_clq(&g, max_clq);
	fflush(stdout);

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
