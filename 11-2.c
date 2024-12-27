#include "prelude.h"

#define DAY    "11"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 65601038650482;

#define ITER       75
#define MAX_CACHED 2024

static usize cache[MAX_CACHED + 1][ITER + 1] = {0};

static usize num_digits(usize num) {
	usize d = 1;
	while ((num /= 10) > 0) d++;
	return d;
}

static void split(usize num, usize len, usize *left, usize *right) {
	assert(num > 0);
	assert(len >= 2);
	assert(len % 2 == 0);
	usize div = 1;
	for (usize d = len / 2; d > 0; d--) div *= 10;
	*left  = num / div;
	*right = num % div;
}

static usize cached(usize (*func)(usize, usize), usize num, usize iter) {
	usize *c = num <= MAX_CACHED ? &cache[num][iter] : NULL;
	if (c && *c) return *c;
	usize result = func(num, iter);
	if (c) *c = result;
	return result;
}

static usize count(usize num, usize iter) {
	if (!iter) return 1;
	if (num == 0) return cached(count, 1, iter - 1);

	usize len = num_digits(num);
	if (len % 2 == 0) {
		usize left, right;
		split(num, len, &left, &right);
		return cached(count, left, iter - 1) +
		       cached(count, right, iter - 1);
	}

	return cached(count, num * 2024, iter - 1);
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	for_each_token(data, t, " \n") {
		usize num  = (usize)atoll(t);
		result    += count(num, ITER);
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
