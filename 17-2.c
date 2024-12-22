// clang-format on
#include "prelude.h"

#define DAY "17"
#define INPUT DAY ".txt"
#define MAX_OUT 32

static u64 find(u8 n, const u8 prog[n], u64 ans) {
	if (!n) return ans;

	u64 a, b, c;
	for (u8 t = 0; t < 8; t++) {
		a = ans << 3 | t;
		b = a % 8;
		b = b ^ 1;
		c = a >> b;
		b = b ^ 5;
		b = b ^ c;
		if (b % 8 == prog[n-1]) {
			u64 sub = find(n-1, prog, a);
			if (sub) return sub;
		}
	}

	return 0;
}

i64 solve(char *data) {
	(void)data;
	const u8 program[] = {
		2, 4, 1, 1, 7, 5, 1, 5, 4, 0, 5, 5, 0, 3, 3, 0,
	};
	i64 result = (i64)find(ASZ(program), program, 0);
	return result;
}

int main(void) {
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
