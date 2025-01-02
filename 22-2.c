// clang-format on
#include "prelude.h"

#define DAY      "22"
#define INPUT    DAY ".txt"
#define SAMPLE   DAY "-s2.txt"
#define MAX_SEQS (1 << 17)
#define LEN      4
#define OFF      (LEN - 1)
#define N        2000

static const i64 expected = 23;

typedef struct {
	i8    seqs[MAX_SEQS][4];
	int   prices[MAX_SEQS];
	usize count;
} Totals;

typedef struct {
	usize idx;
	bool  created;
} Entry;

typedef struct {
	const i8 **keys;
	usize      cap;
	usize      lookups;
	usize      probes;
} HashMap;

static inline bool eq(const i8 a[LEN], const i8 b[LEN]) {
	return !memcmp(a, b, LEN);
}

static usize hash(const i8 key[LEN]) {
	u32 h = 5381;
	for (usize i = 0; i < LEN; i++) h = ((h << 5) + h) + (u32)key[i];
	return h;
}

static Entry lookup(HashMap *hm, const i8 key[LEN]) {
	hm->lookups++;
	usize start = hash(key) % hm->cap;
	for (usize i = 0; i < hm->cap; i++) {
		usize idx = (start + i) % hm->cap;
		if (hm->keys[idx] == NULL)
			return (Entry){.idx = idx, .created = true};
		if (eq(hm->keys[idx], key)) return (Entry){.idx = idx};
		hm->probes++;
	}

	UNREACHABLE("Map full!");
}

static i64 step(i64 number) {
	number ^= number * 64;
	number %= 16777216;
	number ^= number / 32;
	number %= 16777216;
	number ^= number * 2048;
	number %= 16777216;

	return number;
}

i64 solve(char *data) {
	assert(data && "We need data!");

	static Totals totals;
	totals.count = 0;

	static const i8 *keys[CAP(totals.prices)];
	static HashMap   hm = {.keys = keys, .cap = CAP(keys)};
	memset(keys, 0, sizeof keys);
	hm.probes  = 0;
	hm.lookups = 0;

	for_each_line(data, line) {
		i64 number = (i64)atoi(line);
		int price  = number % 10;
		int prev   = price;

		static const i8 *seen_keys[N * 2]; // load factor 0.5
		static HashMap   seen = {
			  .keys = seen_keys, .cap = CAP(seen_keys)};
		memset(seen_keys, 0, sizeof seen_keys);

		static i8 diffs[N];
		for (usize i = 0; i < N; i++) {
			number   = step(number);
			price    = number % 10;
			diffs[i] = (i8)(price - prev);
			prev     = price;

			if (i < OFF) continue;

			// Pointer to the start of the current sequence of <LEN> diffs
			i8 *seq = diffs + (i - OFF);

			Entry entry = lookup(&seen, seq);
			if (!entry.created) continue;
			seen_keys[entry.idx] = seq;

			entry = lookup(&hm, seq);
			if (entry.created) {
				keys[entry.idx] = totals.seqs[entry.idx];
				memcpy(totals.seqs[entry.idx], seq, LEN);
				totals.prices[entry.idx] = 0;
				totals.count++;
			}
			totals.prices[entry.idx] += price;
		}
	}

	printf("Total Seqs  : %zu\n", totals.count);
	printf("Load Factor : %f\n", (f64)totals.count / (f64)hm.cap);
	printf("AVG Probes  : %zu\n", hm.probes / hm.lookups);

	int best = INT_MIN;
	for (usize i = 0; i < hm.cap; i++) {
		if (hm.keys[i] == NULL) continue;
		if (totals.prices[i] > best) best = totals.prices[i];
	}

	return best;
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
