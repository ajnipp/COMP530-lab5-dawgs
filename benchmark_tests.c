// PID: 730477560
// PID: 730461954
// I pledge the COMP530 honor code.

#include "benchmarks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long DEFAULT_SIZE = 1024 * 1024 * 1024; // 1 GB
long DEFAULT_GRANULARITY = 1024 * 1024; // 1 MB

char test_disk_path[256] = DEFAULT_HDD_DISK_PATH;
char test_write_pattern[256] = "P"; // Single character only supported for now

int test_io_size() {
  // Zero terminated array of granularities to test
  long granularities[] = {4 * 1024 * 1024, 100 * 1024 * 1024, 0};

  for (int i = 0; granularities[i] != 0; i++) {
    long result = io_size(READ_M, DEFAULT_SIZE, granularities[i]);
    if (result == -1) {
      return -1;
    }
  }

  set_write_pattern(test_write_pattern);

  for (int i = 0; granularities[i] != 0; i++) {
    long result = io_size(WRITE_M, DEFAULT_SIZE, granularities[i]);
    if (result == -1) {
      return -1;
    }
  }

  // Read the file to ensure that the writes were successful
  FILE *file = fopen(test_disk_path, "r");
  if (file == NULL) {
    perror("fopen");
    return -1;
  }

  char ch;
  char expected = test_write_pattern[0];
  while ((ch = fgetc(file)) != EOF) {
    if (ch != expected) {
      printf("Expected %c but got %c\n", expected, ch);
      return -1;
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  // Get the file name to run tests on from the command line
  if (argc < 3) {
    printf("Usage: %s <file_name> <pattern>\n", argv[0]);
    return 1;
  }

  // Set the disk path to the file name
  set_disk_path(argv[1]);
  strcpy(test_disk_path, argv[1]);

  // Set the write pattern
  strcpy(test_write_pattern, argv[2]);

  // Run the tests
  int res;
  res = test_io_size();
  printf("test_io_size: %s\n", res ? "FAIL" : "PASS");
}
