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
	usize        size;
} HashMap;

static usize       cache_vals[MAX_CACHE];
static const char *cache_keys[MAX_CACHE];

static usize cache_size = 0;
static usize cache_hits = 0;

static HashMap h = {.keys = cache_keys, .size = ASZ(cache_keys)};

static usize hash(const char *k) {
	u32 h = 5381;
	for (; *k; k++) h = ((h << 5) + h) + (u32)*k;
	return h;
}

static isize set(HashMap *h, const char *k) {
	for (usize i = hash(k) % h->size; i < h->size; i++)
		if (h->keys[i] == NULL || !strcmp(h->keys[i], k))
			return (isize)i;
	return -1;
}

static isize get(HashMap *h, const char *k) {
	for (usize i = hash(k) % h->size; i < h->size && h->keys[i]; i++)
		if (!strcmp(h->keys[i], k)) return (isize)i;
	return -1;
}

static inline char *split(char *s, const char *sep) {
	if (!sep[0]) return s;
	s = strstr(s, sep);
	if (!s) return NULL;
	s[0] = '\0';
	s += strlen(sep);
	return s;
}

static usize count_ways(usize np, const char *ps[np], const char *d) {
	if (!*d) return 1;

	isize idx = get(&h, d);
	if (idx != -1) {
		cache_hits++;
		return cache_vals[idx];
	}

	usize count = 0;
	for (usize i = 0; i < np; i++) {
		const char *p = ps[i];
		usize       len = strlen(p);
		if (!strncmp(d, p, len)) count += count_ways(np, ps, d + len);
	}

	idx = set(&h, d);
	assert(idx != -1);
	cache_size++;
	cache_keys[idx] = d;
	cache_vals[idx] = count;

	return count;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;
	cache_size = 0;
	cache_hits = 0;
	memset(cache_keys, 0, sizeof cache_keys);

	char *patterns = data;
	char *designs = split(data, "\n\n");

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

	printf("Cached: %zu\tHits: %zu\n", cache_size, cache_hits);

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
