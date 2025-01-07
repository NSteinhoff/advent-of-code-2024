#include "prelude.h"

#define DAY    "06"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 41;

/// Access character in input data by x,y coordinates
#define CHAR_AT(x, y) (data[y * (w + 1) + x])

enum { N, E, S, W, NUM_DIRS };

typedef struct {
	int x, y;
} P;

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	/// Width
	int w = (int)strcspn(data, "\n");
	/// Height
	int h = (int)strlen(data) / (w + 1);

	assert(h == w && "Square grid expected!");
	printf("Map: %d x %d\n", h, w);

	/// Guard
	P g = {0};
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (CHAR_AT(x, y) == '^') {
				g = (P){x, y};
				break;
			}
		}
	}

	/// Facing direction
	uchar d = N;
	while (true) {
		CHAR_AT(g.x,
		        g.y) = 'X'; // Mark traveled

		/// Target position after stepping in current direction
		P gg = (P[]){
			[N] = {g.x,     g.y - 1},
			[E] = {g.x + 1, g.y    },
			[S] = {g.x,     g.y + 1},
			[W] = {g.x - 1, g.y    },
		}[d];

		if (gg.x < 0 || gg.y < 0 || gg.x >= w || gg.y >= h) {
			break; // Left map
		}

		if (CHAR_AT(gg.x, gg.y) == '#') {
			d = (d + 1) % NUM_DIRS; // Turn clockwise
		} else {
			g = gg; // Step forward
		}
	}

	for (char *p = data; *p; p++)
		if (*p == 'X') result++;

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
