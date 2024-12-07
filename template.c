#include "prelude.h"

#define DAY "{{DAY}}"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const int expected = -1;

int solve(char *data) {
	assert(data && "We need data!");
	int result = 0;

	return result;
}

int main(void) {
	const int actual = solve(read_to_string(SAMPLE));
	printf("Sample: %d\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %d actual %d\n", expected, actual);
		return 1;
	}
	printf("Result: %d\n", solve(read_to_string(INPUT)));
	return 0;
}

