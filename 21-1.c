// clang-format on
#include "prelude.h"

#define DAY    "21"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = -1;

static char numpad[4][3] = {
	"123",
	"456",
	"789",
	".0A",
};

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

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
