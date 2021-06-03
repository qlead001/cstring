.PHONY: clean test tests/%.tester

CC = gcc
CC_FLAGS = -iquote include -O3 -ansi -Wall -Wextra -pedantic -Wformat=2 \
		   -W -Wshadow -Wstrict-prototypes -Wold-style-definition \
		   -D STR_CAP_DEFAULT=128 -D ARR_CAP_DEFAULT=16 -D ERR_SILENT

# Get list of tests
TESTS := $(addsuffix .tester, $(basename $(wildcard tests/*.c)))

test: $(TESTS)

tests/%.tester: %.tester
	./$<

%.tester: tests/%.c str.c
	$(CC) $(CC_FLAGS) -o $@ $^

clean:
	rm -f $(notdir $(TESTS))
