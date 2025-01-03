// clang-format on
#include "prelude.h"

#define DAY    "21"
#define INPUT  DAY ".txt"
#define SAMPLE DAY "-s.txt"

#define MAX_BUTTONS    (3 * 4)
#define MAX_MOVES_DIR  (6 * 5)
#define MAX_MOVES_NUM  (12 * 11)
#define MAX_MOVE_WAYS  12
#define MAX_MOVE_STEPS 12
#define MAX_QUEUE      (1 << 9)
#define MAX_SEQ        256
#define MAX_SEQS       (1 << 21)

static const i64 expected = 126384;

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

typedef struct {
	char *data;
	usize len;
	usize cap;
} Seq;

typedef struct {
	char  data[MAX_SEQS][MAX_SEQ];
	usize count;
	usize best;
} Seqs;

static Move  dirpad_moves[MAX_MOVES_DIR];
static usize dirpad_moves_count;
static Move  numpad_moves[MAX_MOVES_NUM];
static usize numpad_moves_count;

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

	// Create a list of moves from each button to all other buttons,
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

static Seq extended(Seq seq, const char *steps) {
	usize new_len = seq.len + strlen(steps);
	assert(new_len < MAX_SEQ - 1);
	strcpy(&seq.data[seq.len], steps);
	seq.len = new_len;
	return seq;
}

static void collect(Seqs *seqs, const char *seq) {
	assert(seqs->count < MAX_SEQS);
	usize len = strlen(seq);
	assert(len < MAX_SEQ);
	strcpy(seqs->data[seqs->count++], seq);
	if (!seqs->best || len < seqs->best) seqs->best = len;
}

static void collect_sequences(
	char from, const char *input, MoveSet *moveset, Seq seq, Seqs *seqs) {

	if (input[0] == '\0') {
		collect(seqs, seq.data);
		return;
	}

	char  to = input[0];
	Move *m  = find_move(from, to, moveset);
	for (usize k = 0; k < m->num_ways; k++) {
		collect_sequences(
			to, input + 1, moveset, extended(seq, m->ways[k]),
			seqs);
	}
}

static int sequence_length(const char *code) {
	char seq_data[MAX_SEQ] = {0};
	Seq  seq = {.data = seq_data, .len = 0, .cap = sizeof seq_data};

	static Seqs input;
	static Seqs output;
	input.count  = 0;
	output.count = 0;

	MoveSet movesets[] = {
		['n'] = {numpad_moves, numpad_moves_count},
		['d'] = {dirpad_moves, dirpad_moves_count},
	};

	collect(&input, code);

	const uchar types[] = {'n', 'd', 'd', '\0'};
	for (const uchar *t = types; *t; t++) {
		output.count = output.best = 0;
		for (usize i = 0; i < input.count; i++) {
			collect_sequences(
				'A', input.data[i], &movesets[*t], seq,
				&output);
		}

		if (t[1] == '\0') {
			printf("  %s: %8zu ways\n", code, output.count);
			return (int)output.best;
		}

		input.count = 0;
		for (usize i = 0; i < output.count; i++) {
			if (strlen(output.data[i]) == output.best) {
				collect(&input, output.data[i]);
			}
		}
	}
	UNREACHABLE("Should return early!");
}

i64 solve(char *data) {
	assert(data && "We need data!");
	i64 result = 0;

	foreach_line (data, code) {
		int length = sequence_length(code);
		int numval = atoi(code);
		int complx = length * numval;
		printf("  %s: %d\n", code, length);

		result += complx;
	}

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
