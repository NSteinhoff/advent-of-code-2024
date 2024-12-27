.PHONY: help list clean

CC         := clang
CFLAGS     := $(CFLAGS) -std=c17
CFLAGS     += -Wall -Wextra -pedantic
CFLAGS     += -Wconversion -Wmissing-prototypes
COMPILE.c  := $(CC) $(CFLAGS)
CHECK.c    := $(COMPILE.c) -fsyntax-only

SRC_C := $(wildcard *.c)

all: create-new-day check-c

check-c:
	@for f in $(wildcard *.c); do $(CHECK.c) -o "$${f%%.c}" "$${f}"; done
.PHONY: check-c

help:
	@echo
	@echo "usage: make [target]"
	@echo
	@echo "Targets"
	@echo "    <lang>_<day>          Run the solution for <lang> language and <day> day."
	@echo "    list                  List available solutions."
	@echo "    clean                 Remove build artifacts."
	@echo
	@echo "Solutions"
	@make list | column | while read l; do echo "\t$$l"; done

list:
	@find -E . \
		-regex '.*/[0-9]+(-[0-9])?\.[a-z]+' \
		-not -name '*.txt' \
		-not -path '*/target/*' \
		| sed -E 's/.*\/([0-9]+(-[12])?).([a-z]+)/\3_\1/g' \
		| sort -t _ -k 1,1 -k 2,2n

clean:
	@rm -rf rs/target
	@rm -rf *.dSYM
	@rm -f create-new-day
	@find -E . -regex './[0-9]+(-[0-9])?' -delete

py_%: %.py
	@python $<

lua_%: %.lua
	@lua $<

c_%: %.c
	@$(CC) $(CFLAGS) $< -o $*
	@./$*; rm -rf ./$* ./$*.dSYM

rs_%: ./rs/src/bin/%.rs
	@cd ./rs && cargo run --bin $*

js_%: %.js
	@node $<

ts_%: %.ts
	@ts-node $<

%: %.c
	@$(CC) $(CFLAGS) $< -o $@

create-new-day: create-new-day.c
	@$(CC) $(CFLAGS) $< -o $@
