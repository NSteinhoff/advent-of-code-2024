#pragma once

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include <assert.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <string.h>

// Array size
#define ASZ(A) (sizeof(A) / sizeof(A[0]))

// 2-d array access
#define AT(X, Y, W) ((Y) * (W) + (X))
// 2-d string access with newlines as row separators
#define ATS(X, Y, W) ((Y) * (W + 1) + (X))

// clang-format off
typedef size_t        usize;
typedef ptrdiff_t     isize;

typedef unsigned char uchar;
typedef unsigned int  uint;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef float    f32;
typedef double   f64;
// clang-format on

// __LINE__ does not get recursively expanded when part of stringizing # or
// token-pasting ##, so we have to add indirection
#define CONCAT_(prefix, suffix) prefix##suffix
#define CONCAT(prefix, suffix) CONCAT_(prefix, suffix)
// Mangle the variable token so that it is unlikely to clash with another
// identifier
#define MANGLE(name) CONCAT(CONCAT(name##__, __LINE__), __)
#define UNIQ(name) MANGLE(name)

#define for_each_token(S, T, SEP)                                              \
	for (char *UNIQ(brk), *T = strtok_r((S), (SEP), &UNIQ(brk)); T;        \
	     T = strtok_r(NULL, (SEP), &UNIQ(brk)))

#define for_each_line(S, L) for_each_token(S, L, "\n")
#define for_each_word(S, W) for_each_token(S, W, " ")

#define for_in(I, A) for (usize I = 0; I < ASZ(A); I++)
#define for_rg(I, N) for (usize I = 0; I < N; I++)

i64 solve(char *data);

char *read_to_string(const char *restrict fname);
char *read_to_string(const char *restrict fname) {
	FILE *file = fopen(fname, "r");
	if (!file) {
		perror(fname);
		return NULL;
	}

	if (fseek(file, 0L, SEEK_END)) {
		perror("Cannot seek to end of file");
		return NULL;
	}

	long bytes = ftell(file);
	if (bytes == -1) {
		perror("Cannot get position in file");
		return NULL;
	}
	usize bytes_to_read = (usize)bytes;

	if (fseek(file, 0L, SEEK_SET)) {
		perror("Cannot rewind file");
		return NULL;
	}

	char *content = malloc(bytes_to_read + 1);
	if (!content) {
		perror(NULL);
		return NULL;
	}

	usize bytes_read = fread(content, 1, bytes_to_read, file);
	if (bytes_read != bytes_to_read) {
		if (feof(file))
			fprintf(stderr,
			        "Expected to read %zu bytes, but read %zu\n",
			        bytes_to_read, bytes_read);

		if (ferror(file))
			fprintf(stderr, "Could not read all bytes from %s\n",
			        fname);

		free(content);
		return NULL;
	}
	content[bytes_read] = 0;

	if (fclose(file)) {
		perror("Cannot close file");
		free(content);
		return NULL;
	}

	return content;
}
