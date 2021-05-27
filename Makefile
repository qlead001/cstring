.PHONY: clean test tests/%.tester

CC = gcc
CC_FLAGS = -iquote include -Wall

# Get list of tests
TESTS := $(addsuffix .tester, $(basename $(wildcard tests/*.c)))

test: $(TESTS)

tests/%.tester: %.tester
	./$<

%.tester: tests/%.c str.c
	$(CC) $(CC_FLAGS) -o $@ $^

clean:
	rm -f $(notdir $(TESTS))
