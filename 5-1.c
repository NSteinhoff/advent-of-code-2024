#include "prelude.h"

#define DAY "5"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define N 100

static const int expected = 143;

static int rules[N][N] = {0};
static usize num[N] = {0};

int solve(char *data) {
	assert(data && "We need data!");
	int result = 0;
	memset(rules, 0, sizeof rules);
	memset(num, 0, sizeof num);

	char *updates = strstr(data, "\n\n");
	*(updates++) = '\0';
	updates += strspn(updates, "\n");

	for_each_line(data, line) {
		int a, b;
		sscanf(line, "%u|%u", &a, &b);
		rules[a][num[a]++] = b;
	}

	for_each_line(updates, line) {
		int pages[N] = {0};
		usize len = 0;
		for_each_token(line, s, ",") pages[len++] = atoi(s);

		bool valid = true;
		for (usize i = 0; i < len && valid; i++) {
			int a = pages[i];
			for (usize j = 0; j < i && valid; j++) {
				int b = pages[j];
				for (usize k = 0; k < num[a] && valid; k++) {
					if (b == rules[a][k]) valid = false;
				}
			}
		}
		if (valid) result += pages[len / 2];
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
