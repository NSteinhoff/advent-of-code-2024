#include "prelude.h"

#define DAY "09"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"

// 6390180901651
static const i64 expected = 1928;

#define LEN(i) ((u8)data[i] - (u8)'0')

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	usize n = (strcspn(data, "\n") + 1) / 2;

	usize last = n - 1;
	u8 last_len = LEN(last * 2);

	usize block = 0;
	for (usize file = 0; file < n; file++) {
		// File
		u8 file_len = file == last ? last_len : LEN(file * 2);
		while (file_len-- > 0) result += file * block++;
		if (file == last) break;

		// Free
		u8 free_len = LEN(file * 2 + 1);
		while (free_len-- > 0) {
			if (last_len == 0) {
				if (--last == file) return result;
				last_len = LEN(last * 2);
			}
			if (last_len == 0) break;
			result += last * block++;
			last_len--;
		}
	};

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
