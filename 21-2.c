// clang-format on
#include "prelude.h"

#define DAY    "21"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_CODES      8
#define MAX_BUTTONS    (3 * 4)
#define MAX_MOVES_DIR  (6 * 5)
#define MAX_MOVES_NUM  (12 * 11)
#define MAX_MOVE_WAYS  12
#define MAX_MOVE_STEPS 12
#define MAX_QUEUE      (1 << 9)
#define MAX_CACHE      (1 << 9)

static const i64 expected = 154115708116294;

typedef struct {
	int x, y;
} P;

typedef struct {
	P    p;
	u8   len;
	char steps[MAX_MOVE_STEPS];
} Path;

typedef struct {
	char c;
	P    p;
} Button;

typedef struct {
	Button from, to;
	usize  num_ways;
	char   ways[MAX_MOVE_WAYS][MAX_MOVE_STEPS + 1];
} Move;

typedef struct {
	Move *moves;
	usize count;
} MoveSet;

static Move  dirpad_moves[MAX_MOVES_DIR];
static usize dirpad_moves_count;
static Move  numpad_moves[MAX_MOVES_NUM];
static usize numpad_moves_count;

typedef struct {
	char  from, to;
	usize depth;
	usize len;
} CacheEntry;
static CacheEntry cache[MAX_CACHE];
static usize      cache_count = 0;

// clang-format off
static const char *const numpad[] = {
	".....",
	".789.",
	".456.",
	".123.",
	"..0A.",
	".....",
	NULL,
};

static const char *const dirpad[] = {
	".....",
	"..^A.",
	".<v>.",
	".....",
	NULL,
};
// clang-format on

static inline bool eq(const P *p1, const P *p2) {
	return p1->x == p2->x && p1->y == p2->y;
}

static inline Path step(const Path *path, char direction) {
	Path next              = *path;
	next.steps[next.len++] = direction;

	switch (direction) {
	case '^': next.p.y--; break;
	case 'v': next.p.y++; break;
	case '>': next.p.x++; break;
	case '<': next.p.x--; break;
	default: UNREACHABLE("Invalid direction!");
	}

	return next;
}

static usize
compute_moves(const char *const pad[], usize cap, Move moves[cap]) {
	Button buttons[MAX_BUTTONS] = {0};
	usize  num_buttons          = 0;

	// Collect the buttons of the pad with label and coordinates
	for (int y = 0; pad[y]; y++) {
		for (int x = 0; pad[y][x]; x++) {
			if (pad[y][x] == '.') continue;
			assert(num_buttons < CAP(buttons));
			buttons[num_buttons++] = (Button){
				.c = pad[y][x],
				.p = {.x = x, .y = y},
			};
		}
	}

	// Create a list of moves from each button to all other buttons
	// i.e. Cartensian Product with itself.
	usize len = 0;
	for (usize i = 0; i < num_buttons; i++) {
		for (usize j = 0; j < num_buttons; j++) {
			assert(len < cap);
			moves[len++] = (Move){
				.from = buttons[i],
				.to   = buttons[j],
			};
		}
	}

	// Compute all the shortest ways to make the move
	for (usize i = 0; i < len; i++) {
		Move *move = &moves[i];
		u8    len  = 0xff;

		Path  paths[MAX_QUEUE];
		Queue q          = {.cap = CAP(paths)};
		paths[qpush(&q)] = (Path){.p = move->from.p};

		while (q.len) {
			Path path = paths[qpop(&q)];
			if (path.len > len) continue;
			if (eq(&path.p, &move->to.p)) {
				if (len > path.len) len = path.len;
				assert(path.len == len);
				assert(move->num_ways < CAP(move->ways));
				path.steps[path.len++] = 'A';
				path.steps[path.len]   = '\0';
				strcpy(move->ways[move->num_ways++],
				       path.steps);
			}

			const char *dirs = "^v<>";
			for (usize i = 0; i < strlen(dirs); i++) {
				Path next = step(&path, dirs[i]);
				if (pad[next.p.y][next.p.x] == '.') continue;
				paths[qpush(&q)] = next;
			}
		}
	}

	return len;
}

static void precompute_moves(void) {
	numpad_moves_count =
		compute_moves(numpad, CAP(numpad_moves), numpad_moves);
	dirpad_moves_count =
		compute_moves(dirpad, CAP(dirpad_moves), dirpad_moves);
}

static Move *find_move(char from, char to, const MoveSet *moveset) {
	for (usize i = 0; i < moveset->count; i++) {
		if (moveset->moves[i].from.c == from
		    && moveset->moves[i].to.c == to)
			return &moveset->moves[i];
	}
	UNREACHABLE("Cannot find move!");
}

static usize get_length(char from, char to, usize depth, usize max_depth) {
	// Check for cached result. We do a linear scan, because we only have
	// ~500 entries in the cache. Using a hash table would be overkill.
	for (usize i = 0; i < cache_count; i++) {
		if (cache[i].from == from && cache[i].to == to
		    && cache[i].depth == depth)
			return cache[i].len;
	}

	// Grab the keypad moveset based on the depth
	const MoveSet moveset =
		depth == 0 ? (MoveSet){numpad_moves, numpad_moves_count}
			   : (MoveSet){dirpad_moves, dirpad_moves_count};

	// Get the pre-computed move data
	const Move *m = find_move(from, to, &moveset);

	usize best = SIZE_MAX;
	for (usize i = 0; i < m->num_ways; i++) {
		const char *way = m->ways[i];
		usize       len = 0;
		if (depth == max_depth) {
			// When we are at max depth, the length in simply the
			// number of steps
			len = strlen(way);
		} else {
			// When we are not at max depth, then we have to
			// recursively expand the move and collect the best
			// length at each level
			char from = 'A';
			for (usize j = 0; j < strlen(way); j++) {
				char to  = way[j];
				len     += get_length(
                                        from, to, depth + 1, max_depth);
				from = to;
			}
		}
		assert(len);
		if (len < best) best = len;
	}

	assert(cache_count < MAX_CACHE);
	cache[cache_count++] = (CacheEntry){
		.to    = to,
		.from  = from,
		.depth = depth,
		.len   = best,
	};

	return best;
}

static u64 sequence_length(const char *code) {
	usize n_robots = 25;

	u64  len  = 0;
	char from = 'A';
	for (usize i = 0; i < strlen(code); i++) {
		char to = code[i];

		len += get_length(from, to, 0, n_robots);

		from = to;
	}

	return len;
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	for_each_line(data, code) {
		u64 length = sequence_length(code);
		u64 numval = (u64)atoi(code);
		u64 complx = length * numval;
		printf("  %s: %llu\n", code, length);

		result += complx;
	}
	printf("Cached: %zu\n", cache_count);

	return result;
}

int main(void) {
	precompute_moves();
	const i64 actual = solve(read_to_string(SAMPLE));
	printf("Sample: %lld\n", actual);
	if (actual != expected) {
		printf("FAIL: expected %lld actual %lld\n", expected, actual);
		return 1;
	}
	printf("Result: %lld\n", solve(read_to_string(INPUT)));
	return 0;
}
