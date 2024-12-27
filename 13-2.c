#include "prelude.h"

#define DAY    "13"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 875318608908;

static i64 calculate_cost(i64 ax, i64 ay, i64 bx, i64 by, i64 px, i64 py) {
	// a*ax + b*bx = px
	// a*ay + b*by = py
	//
	// a*ax*by + b*bx*by = px*by
	// a*ay*bx + b*by*bx = py*bx
	//
	// a*ax*by + b*bx*by - a*ay*bx - b*by*bx  = px*by - py*bx
	// a*ax*by           - a*ay*bx            = px*by - py*bx
	// a * (ax*by - ay*bx)                    = px*by - py*bx
	// a                                      = (px*by - py*bx) / (ax*by - ay*bx)
	// a*ax + b*bx = px
	//       b*bx  = px - a*ax
	//       b     = (px - a*ax) / bx
	//
	// a = (px*by - py*bx) / (ax*by - ay*bx)
	// b = (px - a*ax) / bx
	assert(ax * by - ay * bx != 0);
	i64 a  = (px * by - py * bx) / (ax * by - ay * bx);
	i64 ra = (px * by - py * bx) % (ax * by - ay * bx);
	i64 b  = (px - a * ax) / bx;
	i64 rb = (px - a * ax) % bx;

	if (ra || rb) return 0;

	return a * 3 + b;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	i64 c = 10000000000000;

	for (char *s = data; s; (s = strstr(s, "\n\n")) && (s += 2)) {
		int ax, ay, bx, by, px, py;
		sscanf(s, "Button A: X%d, Y%d", &ax, &ay);
		s = strchr(s, '\n') + 1;
		sscanf(s, "Button B: X%d, Y%d", &bx, &by);
		s = strchr(s, '\n') + 1;
		sscanf(s, "Prize: X=%d, Y=%d", &px, &py);

		i64 cost = calculate_cost(
			(i64)ax, (i64)ay, (i64)bx, (i64)by, (i64)px + c,
			(i64)py + c);

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
