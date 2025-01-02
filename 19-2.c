// clang-format on
#include "prelude.h"

#define DAY    "19"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_PATTERNS 500
#define MAX_DESIGNS  400
#define MAX_STACK    64
#define MAX_CACHE    (1 << 14)

static const i64 expected = 16;

typedef struct {
	const char **keys;
	usize        cap;
} HashMap;

typedef struct {
	usize idx;
	bool  created;
} Entry;

static usize       cache_vals[MAX_CACHE];
static const char *cache_keys[MAX_CACHE];

static usize cache_size = 0;
static usize cache_hits = 0;

static HashMap h = {.keys = cache_keys, .cap = CAP(cache_keys)};

static usize hash(const char *k) {
	u32 h = 5381;
	for (; *k; k++) h = ((h << 5) + h) + (u32)*k;
	return h;
}

static Entry lookup(HashMap *h, const char *k) {
	usize start = hash(k) % h->cap;

	for (usize i = 0; i < h->cap; i++) {
		usize idx = start + i;
		if (h->keys[idx] == NULL)
			return (Entry){
				.idx     = idx,
				.created = true,
			};
		if (!strcmp(h->keys[idx], k)) return (Entry){.idx = idx};
	}

	UNREACHABLE("Hash map full!");
}

static inline char *split(char *s, const char *sep) {
	if (!sep[0]) return s;
	s = strstr(s, sep);
	if (!s) return NULL;
	s[0]  = '\0';
	s    += strlen(sep);
	return s;
}

static usize count_ways(usize np, const char *ps[np], const char *d) {
	if (!*d) return 1;

	Entry entry = lookup(&h, d);
	if (!entry.created) {
		cache_hits++;
		return cache_vals[entry.idx];
	}

	usize count = 0;
	for (usize i = 0; i < np; i++) {
		const char *p   = ps[i];
		usize       len = strlen(p);
		if (!strncmp(d, p, len)) count += count_ways(np, ps, d + len);
	}

	cache_size++;
	cache_keys[entry.idx] = d;
	cache_vals[entry.idx] = count;

	return count;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;
	cache_size = 0;
	cache_hits = 0;
	memset(cache_keys, 0, sizeof cache_keys);

	char *patterns = data;
	char *designs  = split(data, "\n\n");

	const char *ps[MAX_PATTERNS], *ds[MAX_DESIGNS];
	usize       np = 0, nd = 0;

	for_each_token(patterns, p, ", ") {
		assert(np < MAX_PATTERNS);
		ps[np++] = p;
	}
	for_each_token(designs, d, "\n") {
		assert(nd < MAX_DESIGNS);
		ds[nd++] = d;
	}

	printf("Patterns: %zu\n", np);
	printf("Designs: %zu\n", nd);

	usize count = 0;
	for (usize i = 0; i < nd; i++) count += count_ways(np, ps, ds[i]);
	result = (i64)count;

	printf("Cached: %zu\tHits: %zu\tLoad Factor: %f\n", cache_size,
	       cache_hits, (f64)cache_size / (f64)MAX_CACHE);

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
