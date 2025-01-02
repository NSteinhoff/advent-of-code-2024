// clang-format on
#include "prelude.h"

#define DAY    "22"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 37327623;

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
	i64 result = 0;

	for_each_line(data, line) {
		i64 number = (i64)atoi(line);
		for (usize i = 0; i < 2000; i++) number = step(number);
		result += number;
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
