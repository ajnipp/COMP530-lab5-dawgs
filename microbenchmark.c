#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "benchmarks.h"

void help() {
  printf("Usage: mbench <test> [options]\n");
  printf("Options:\n");
  printf("  -d <str> disk path to test\n");
  printf("  -t <int> total size of test write (1GB default)\n");
  printf("  -g <int> granularity of test write (1MB default)\n");
  printf("  -p <str> write pattern to use (P default)\n");
  printf("  -s <int> stride of test write (0 bytes default)\n");
  printf("  -l <int> lower bound of random write offset (0 bytes default)\n");
  printf("  -u <int> upper bound of random write offset (1 GB default)\n");
  printf("  -w Perform write test (default is read)\n");
}

// Define options
char *arg_disk_path = "/dev/sda";
long total_size = 1024 * 1024 * 1024; // 1 GB
long granularity = 1024 * 1024; // 1 MB
char *arg_write_pattern = "P"; // Single character only supported for now
long stride = 0;
long min_offset = 0;
long max_offset = 1024 * 1024 * 1024; // 1 GB
int mode = READ_M;

int main(int argc, char **argv) {
  int test_to_run;

    if (argc < 2) {
        help();
        return 1;
    }

  char* test_name = argv[1];
    if (strcmp(test_name, "size") == 0) {
        test_to_run = 0;
    } else if (strcmp(test_name, "stride") == 0) {
        test_to_run = 1;
    } else if (strcmp(test_name, "random") == 0) {
        test_to_run = 2;
    } else {
        help();
        return 1;
    }

  // Parse options
  int c;
  while ((c = getopt(argc, argv, "d:t:g:p:s:l:u:w")) != -1) {
    switch (c) {
      case 'd':
        arg_disk_path = optarg;
        break;
      case 't':
        total_size = atol(optarg);
        break;
      case 'g':
        granularity = atol(optarg);
        break;
      case 'p':
        arg_write_pattern = optarg;
        break;
      case 's':
        stride = atol(optarg);
        break;
      case 'l':
        min_offset = atol(optarg);
        break;
      case 'u':
        max_offset = atol(optarg);
        break;
      case 'w':
        mode = WRITE_M;
        break;
      default:
        help();
        return 1;
    }
  }

  // Initialize options for test
  set_disk_path(arg_disk_path);
  set_write_pattern(arg_write_pattern);

  // Run the test
  long res;
  switch (test_to_run) {
    case 0:
      res = io_size(mode, total_size, granularity);
      break;
    case 1:
        res = io_stride(mode, total_size, granularity, stride);
        break;
    case 2:
        res = random_io(mode, total_size, granularity, min_offset, max_offset);
        break; 
  }

  printf("%ld ms\n", res);
  return 0;
}
