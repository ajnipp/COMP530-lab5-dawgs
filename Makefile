CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = benchmark_tests
SRCS = benchmark_tests.c benchmarks.c microbenchmark.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
