#include "prelude.h"

#define DAY "3"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

const int expected = 48;

int solve(char *data) {
	assert(data && "We need data!");
	int result = 0;
	printf("%s\n", data);
	usize n = strlen(data);
	bool enabled = true;
	for (usize i = 0; i < n; i++) {
		if (!strncmp(data + i, "do()", 4)) {
			enabled = true;
			continue;
		}
		if (!strncmp(data + i, "don't()", 7)) {
			enabled = false;
			continue;
		}

		if (!enabled) continue;
		if (strncmp(data + i, "mul(", 4)) continue;

		printf("%.4s @ %zu\n", data + i, i);
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

		usize len = ii - i + 1;
		printf(">> %.*s -> %d * %d\n", (int)len, data + i, x, y);
	}

	return result;
}

int main(void) {
	const int actual = solve(read_to_string(SAMPLE));
	printf("Example: %d\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %d actual %d\n", expected, actual);
		return 1;
	}
	printf("Result: %d\n", solve(read_to_string(INPUT)));
	return 0;
}
