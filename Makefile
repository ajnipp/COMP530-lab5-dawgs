CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

TOOL = mbench
TEST = benchmark_tests
TOOL_SRCS = microbenchmark.c benchmarks.c
TEST_SRCS = benchmark_tests.c benchmarks.c
TOOL_OBJS = $(TOOL_SRCS:.c=.o)
TEST_OBJS = $(TEST_SRCS:.c=.o)

.PHONY: all clean tool test

all: tool test

tool: $(TOOL)

test: $(TEST)

$(TOOL): $(TOOL_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(TEST): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TOOL) $(TEST) $(TOOL_OBJS) $(TEST_OBJS)
