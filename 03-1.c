#include "prelude.h"

#define DAY    "03"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

const i64 expected = 161;

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	usize n = strlen(data);
	for (usize i = 0; i < n; i++) {
		if (strncmp(data + i, "mul(", 4)) continue;
		usize ii = i + 4;

		char *arg = data + ii;
		for (; ii < n && isdigit(data[ii]); ii++);
		if (data[ii] != ',') continue;
		ii++;
		int x = atoi(arg);

		arg = data + ii;
		for (; ii < n && isdigit(data[ii]); ii++);
		if (data[ii] != ')') continue;
		int y = atoi(arg);

		result += x * y;
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
