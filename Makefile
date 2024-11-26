CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

TEST = benchmark_tests
SRCS = benchmark_tests.c benchmarks.c 
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

test: $(TEST)

$(TEST): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
