#include "prelude.h"

#define DAY "07"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 11387;
#define MAX_OPS 32

static i64 concat(i64 left, i64 right) {
	static char buf[64];
	int len = snprintf(buf, sizeof buf, "%lld%lld", left, right);
	assert(len > 0 || (usize)len < sizeof buf - 1);
	return strtoll(buf, NULL, 10);
}

static bool check(usize n, i64 *ops, i64 target, i64 total) {
	if (!n) return total == target;

	return check(n - 1, ops + 1, target, total + ops[0]) ||
	       check(n - 1, ops + 1, target, total * ops[0]) ||
	       check(n - 1, ops + 1, target, concat(total, ops[0]));
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	for_each_line(data, line) {
		char *brk;
		char *value = strtok_r(line, ": ", &brk);
		i64 target = strtoll(value, NULL, 10);
		i64 ops[MAX_OPS];
		usize n = 0;
		for (n = 0; n < MAX_OPS; n++) {
			char *op = strtok_r(NULL, " ", &brk);
			if (!op) break;
			ops[n] = strtoll(op, NULL, 10);
		};
		assert(n < MAX_OPS);
		if (check(n - 1, ops + 1, target, ops[0])) result += target;
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
