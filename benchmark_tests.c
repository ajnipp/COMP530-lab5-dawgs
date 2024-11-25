#include "benchmarks.h"
#include <stdio.h>
#include <stdlib.h>

long DEFAULT_SIZE = 1024 * 1024 * 1024; // 1 GB
long DEFAULT_GRANULARITY = 1024 * 1024; // 1 MB

char* disk_path = DEFAULT_HDD_DISK_PATH;

int main(int argc, char **argv) {
    // Get the file name to run tests on from the command line
    if (argc < 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    // Set the disk path to the file name
    set_disk_path(argv[1]);
    disk_path = argv[1];

    // Run the tests
    int res;
    res = test_io_size();
    printf("test_io_size: %s\n", res ? "PASS" : "FAIL");
}

int test_io_size() {
    long granularities[] = {4 * 1024, 8 * 1024, 256 * 1024, 1024 * 1024, 4 * 1024 * 1024, 100 * 1024 * 1024, NULL};

    for (int i = 0; granularities[i] != NULL; i++) {
        long result = io_size(READ_M, DEFAULT_SIZE, granularities[i]);
        if (result == -1) {
            return -1;
        }
    }

    set_write_pattern("P");

    for (int i = 0; granularities[i] != NULL; i++) {
        long result = io_size(WRITE_M, DEFAULT_SIZE, granularities[i]);
        if (result == -1) {
            return -1;
        }
    }

    // Read the file to ensure that the writes were successful
    FILE *file = fopen(disk_path, "r");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    char ch;
    while (ch = fgetc(file) != EOF) {
        if (ch != 'P') {
            return -1;
        }
    }

    return 0;
}