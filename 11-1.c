#include "prelude.h"

#define DAY    "11"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 55312;

#define N    100
#define ITER 25

typedef struct {
	usize num, iter;
} Stone;

static usize num_digits(usize num) {
	usize d = 1;
	while ((num /= 10) > 0) d++;
	return d;
}

static void split(usize num, usize len, usize *left, usize *right) {
	assert(num > 0);
	assert(len >= 2);
	assert(len % 2 == 0);
	usize div = 1;
	for (usize d = len / 2; d > 0; d--) div *= 10;
	*left  = num / div;
	*right = num % div;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	Stone stones[N];
	usize n = 0;

	foreach_token (data, t, " \n") {
		assert(n < N);
		usize num = (usize)atoll(t);
		stones[n] = (Stone){.num = num, .iter = ITER};
		n++;
	}

	while (n--) {
		Stone s   = stones[n];
		usize len = num_digits(s.num);

		if (s.iter == 0) {
			result++;
		} else if (s.num == 0) {
			stones[n++] = (Stone){.num = 1, .iter = s.iter - 1};
		} else if (len % 2 == 0) {
			usize left, right;
			split(s.num, len, &left, &right);
			stones[n++] = (Stone){.num = left, .iter = s.iter - 1};
			stones[n++] = (Stone){.num = right, .iter = s.iter - 1};
		} else {
			stones[n++] = (Stone){.num  = s.num * 2024,
			                      .iter = s.iter - 1};
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
