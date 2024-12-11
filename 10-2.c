#include "prelude.h"

#define DAY "10"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_STARTS 500
#define MAX_STACK 10

static const i64 expected = 81;

typedef struct {
	int x, y;
} Pos;

#define HEIGHT(x, y) (data[ATS((x), (y), nrows)])

Pos dirs[] = {
	{.x = 1,  .y = 0 },
	{.x = -1, .y = 0 },
	{.x = 0,  .y = 1 },
	{.x = 0,  .y = -1},
};

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	int len = (int)strlen(data);
	int ncols = (int)strcspn(data, "\n");
	int nrows = len / (ncols + 1);

	printf("Grid: %d x %d\n", nrows, ncols);

	Pos starts[MAX_STARTS];
	int n_starts = 0;

	for (int y = 0; y < nrows; y++) {
		for (int x = 0; x < ncols; x++) {
			if (HEIGHT(x, y) == '0') {
				assert(n_starts < MAX_STARTS);
				starts[n_starts++] = (Pos){.x = x, .y = y};
			}
		}
	}

	for (int i = 0; i < n_starts; i++) {
		Pos stack[MAX_STACK];
		int top = 0;

		// Depth-first
		stack[top++] = starts[i];
		while (top > 0) {
			Pos p = stack[--top];
			int height = HEIGHT(p.x, p.y);
			if (height == '9') {
				result++;
				continue;
			}

			for (int j = 0; j < 4; j++) {
				int x = p.x + dirs[j].x;
				int y = p.y + dirs[j].y;

				if (x < 0 || y < 0 || x >= ncols ||
				    y >= nrows || HEIGHT(x, y) - height != 1) {
					continue;
				}

				assert(top < MAX_STACK);
				stack[top++] = (Pos){.x = x, .y = y};
			}
		}
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
