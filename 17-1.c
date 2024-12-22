// clang-format on
#include "prelude.h"

#define DAY "17"
#define INPUT DAY ".txt"
#define SAMPLE DAY "-s.txt"
#define MAX_INSTRUCTIONS 32
#define MAX_PROGRAM 256
#define MAX_OUT 256

static const i64 expected = 600;

static struct {
	uint a, b, c;
	uint pc;
	uint len;
	u8 instr[MAX_INSTRUCTIONS];
	char out[MAX_OUT];
} G;

static void print(void) {
	printf(" -- A: %u B: %u C: %u\n", G.a, G.b, G.c);
	printf("    [");
	for (usize i = 0; i < G.len; i++) {
		if (i == G.pc)
			printf("(%u,", G.instr[i]);
		else if (i == G.pc + 1)
			printf(" %u)", G.instr[i]);
		else
			printf(" %u ", G.instr[i]);
	}
	printf("]");
	printf("\tOUT: %s\n", G.out);
}

static inline uint combo(u8 p) {
	assert(p < 7 && "Invalid combo operand!");
	return p == 4 ? G.a : p == 5 ? G.b : p == 6 ? G.c : p;
}

static inline bool out(uint v) {
	int len = snprintf(G.out, sizeof G.out, "%s%s%d", G.out,
	                   G.out[0] ? "," : "", v);
	assert(len > 0 && (usize)len < sizeof G.out && "Invalid output!");
	return false;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;
	memset(&G, 0, sizeof G);

	const char *c = data;
	char prog[MAX_PROGRAM] = {0};
	sscanf(c,
	       "Register A: %d\nRegister B: %d\nRegister C:%d\nProgram: %63[0-9,]\n",
	       &G.a, &G.b, &G.c, prog);
	for (char *c = prog; *c; c++) {
		if (*c == ',') continue;
		u8 op = (u8)*c - '0';
		assert(G.len < MAX_INSTRUCTIONS);
		assert(op <= 7 && "Invalid op code!");
		G.instr[G.len++] = op;
	}

	print();
	while (G.pc < G.len) {
		u8 o = G.instr[G.pc];
		u8 p = G.instr[G.pc + 1];

		if (o == 0 /* adv */) {
			G.a = G.a >> combo(p);
		} else if (o == 1 /* bxl */) {
			G.b ^= p;
		} else if (o == 2 /* bst */) {
			G.b = combo(p) & 0x7;
		} else if (o == 3 /* jnz */) {
			if (G.a != 0) {
				G.pc = p;
				continue;
			}
		} else if (o == 4 /* bxc */) {
			G.b ^= G.c;
		} else if (o == 5 /* out */) {
			out(combo(p) & 0x7);
		} else if (o == 6 /* bdv */) {
			G.b = G.a >> combo(p);
		} else if (o == 7 /* cdv */) {
			G.c = G.a >> combo(p);
		}

		G.pc += 2;
	}
	print();

	result = G.a + G.b + G.c + G.pc * 100;

	return result;
}

int main(void) {
	const i64 actual = solve(read_to_string(SAMPLE));
	assert(strcmp(G.out, "4,6,3,5,6,3,5,2,1,0") == 0);
	printf("Sample: %lld\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %lld actual %lld\n", expected, actual);
		return 1;
	}
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
