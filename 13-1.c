#include "prelude.h"

#define DAY "13"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 480;

static int calculate_cost(int ax, int ay, int bx, int by, int px, int py) {
	int cost = 0;
	for (int a = 0; a <= 100; a++) {
		for (int b = 0; b <= 100; b++) {
			if (a * ax + b * bx == px && a * ay + b * by == py) {
				int c = a * 3 + b;
				if (!cost || c < cost) cost = c;
			}
		}
	}
	return cost;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	for (char *s = data; s; (s = strstr(s, "\n\n")) && (s += 2)) {
		int ax, ay, bx, by, px, py;
		sscanf(s, "Button A: X%d, Y%d", &ax, &ay);
		s = strchr(s, '\n') + 1;
		sscanf(s, "Button B: X%d, Y%d", &bx, &by);
		s = strchr(s, '\n') + 1;
		sscanf(s, "Prize: X=%d, Y=%d", &px, &py);

		int cost = calculate_cost(ax, ay, bx, by, px, py);
		printf("a=[%d,%d] b=[%d,%d] p=[%d,%d] -> %d\n", ax, ay, bx, by,
		       px, py, cost);

		result += cost;
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
