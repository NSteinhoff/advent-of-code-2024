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

// Array capacity
#define CAP(A) (sizeof(A) / sizeof(A[0]))

// 2-d array access
#define AT(X, Y, W)  ((Y) * (W) + (X))
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

#define UNREACHABLE(MSG) assert(false && MSG)

// __LINE__ does not get recursively expanded when part of stringizing # or
// token-pasting ##, so we have to add indirection
#define CONCAT_(prefix, suffix) prefix##suffix
#define CONCAT(prefix, suffix)  CONCAT_(prefix, suffix)

// Mangle the variable token so that it is unlikely to clash with another
// identifier
#define MANGLE(name) CONCAT(CONCAT(name##__, __LINE__), __)
#define UNIQ(name)   MANGLE(name)

#define foreach_token(S, T, SEP)                                               \
	for (char *UNIQ(brk), *T = strtok_r((S), (SEP), &UNIQ(brk)); T;        \
	     T = strtok_r(NULL, (SEP), &UNIQ(brk)))

#define foreach_line(S, L) foreach_token (S, L, "\n")
#define foreach_word(S, W) foreach_token (S, W, " ")

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

char *strsplit(char **s, const char *sep);
char *strsplit(char **s, const char *sep) {
	char *ret = *s;
	if (!sep[0]) return ret;
	char *w = strstr(*s, sep);
	if (!w) {
		*s = strchr(*s, '\0');
	} else {
		w[0] = '\0';
		*s   = w + strlen(sep);
	}
	return ret;
}

typedef struct {
	usize cap, len;
} Stack;

typedef struct {
	usize s, e, cap, len;
} Queue;

usize spush(Stack *s);
usize spush(Stack *s) {
	assert(s->len < s->cap && "Stack full!");
	return s->len++;
}

usize spop(Stack *s);
usize spop(Stack *s) {
	assert(s->len && "Stack empty!");
	return --s->len;
}

usize qpush(Queue *q);
usize qpush(Queue *q) {
	assert(q->len < q->cap && "Queue full!");
	usize i = q->e;
	q->e    = q->e < q->cap - 1 ? q->e + 1 : 0;
	q->len++;
	return i;
}

usize qpop(Queue *q);
usize qpop(Queue *q) {
	assert(q->len > 0 && "Queue empty!");
	usize i = q->s;
	q->s    = q->s < q->cap - 1 ? q->s + 1 : 0;
	q->len--;
	return i;
}
