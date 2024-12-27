#include "prelude.h"

#define DAY    "09"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

static const i64 expected = 2858;

#define LEN(x) ((u8)x - (u8)'0')

#define MAX_BLOCKS 20 * 1000 * 10

typedef struct {
	int file; // -1 for free space
	u8 len; // Counting up from 0 for files and down from max 'len' for free space
} Block;

static Block blocks[MAX_BLOCKS];

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	memset(blocks, 0, sizeof blocks);
	usize n = strcspn(data, "\n");

	usize n_blocks = 0;
	for (usize i = 0; i < n; i++) {
		u8  len  = LEN(data[i]);
		int file = i % 2 == 0 ? (int)i / 2 : -1;

		for (u8 j = 0; j < len; j++) {
			blocks[n_blocks++] = (Block){
				.file = file, .len = file == -1 ? len - j : j};
		}
	};

	for (usize i = 0; i < n_blocks; i++) {
		// Find start of next file from the end
		usize  ii = n_blocks - 1 - i;
		Block *b  = &blocks[ii];
		if (b->file == -1 || b->len != 0) continue;

		// Find the length of the file
		u8 len = 0;
		for (usize j = 0; j < n_blocks; j++) {
			if (blocks[ii + j].file != b->file) break;
			len++;
		}

		// Find the first free block that can fit the file
		Block *bb = NULL;
		for (usize j = 0; j < ii; j++) {
			if (blocks[j].file == -1 && blocks[j].len >= len)
				bb = &blocks[j];
			if (bb) break;
		}

		// Skip if none found
		if (bb == NULL) continue;

		// Move the block
		for (u8 j = 0; j < len; j++) {
			bb[j] = b[j];
			b[j]  = (Block){.file = -1, .len = len - j};
		}
	}

	// Compute checksum
	for (usize i = 0; i < n_blocks; i++) {
		Block *b = &blocks[i];
		if (b->file != -1) {
			result += (usize)b->file * i;
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
