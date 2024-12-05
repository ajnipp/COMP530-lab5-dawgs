// PID: 730477560
// PID: 730461954
// I pledge the COMP530 honor code.

#define _GNU_SOURCE
#include "benchmarks.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_PATH 256

char disk_path[MAX_PATH] = DEFAULT_HDD_DISK_PATH;
char write_pattern[MAX_PATH] = DEFAULT_PATTERN;

int open_disk();
int close_disk();

int set_disk_path(char *path) {
  strcpy(disk_path, path);
  return 1;
}

int set_write_pattern(char *pattern) {
  strcpy(write_pattern, pattern);
  return 1;
}

long io_size(int read, long total_size, long granularity) {
  long result = io_stride(read, total_size, granularity, granularity);
  return result;
}

long io_stride(int read, long total_size, long granularity, long stride) {
  int fd = open_disk();
  if (fd == -1) {
    return -1;
  }

  char *buf = (char *)malloc(granularity);
  int write_pattern_len = strlen(write_pattern);

  for (int i = 0; i < granularity; i += write_pattern_len) {
    memcpy(buf + i, write_pattern, write_pattern_len);
  }

  int num_writes = total_size / granularity;

  // Record the time before starting the reads/writes
  struct timeval start, end;
  gettimeofday(&start, NULL);

  for (int i = 0; i < num_writes; i++) {
    int offset = i * stride;
    if (read) {
      if (pread(fd, buf, granularity, offset) == -1) {
        perror("pread");
        return -1;
      }
    } else {
      if (pwrite(fd, buf, granularity, offset) == -1) {
        perror("pwrite");
        return -1;
      }
      fsync(fd);
    }
  }

  // Record the time after finishing the reads/writes
  gettimeofday(&end, NULL);
  long mtime, seconds, useconds;
  seconds = end.tv_sec - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  mtime = ((seconds)*1000 + useconds / 1000.0);

  free(buf);
  close_disk(fd);

  return mtime;
}

long random_io(int read, long total_size, long granularity, long min_offset,
               long max_offset) {
  int fd = open_disk();
  if (fd == -1) {
    return -1;
  }

  char *buf = (char *)malloc(granularity);
  int write_pattern_len = strlen(write_pattern);

  for (int i = 0; i < granularity; i += write_pattern_len) {
    memcpy(buf + i, write_pattern, write_pattern_len);
  }

  int num_writes = total_size / granularity;

  // Compute an array of offsets to write to, within the range [min_offset,
  // max_offset]
  long offsets[num_writes];
  for (int i = 0; i < num_writes; i++) {
    offsets[i] = min_offset + (rand() % (max_offset - min_offset));
  }

  // Record the time before starting the reads/writes
  struct timeval start, end;
  gettimeofday(&start, NULL);

  for (int i = 0; i < num_writes; i++) {
    if (read) {
      if (pread(fd, buf, granularity, offsets[i]) == -1) {
        perror("pread");
        return -1;
      }
    } else {
      if (pwrite(fd, buf, granularity, offsets[i]) == -1) {
        perror("pwrite");
        return -1;
      }
      fsync(fd);
    }
  }

  // Record the time after finishing the reads/writes
  gettimeofday(&end, NULL);
  long mtime, seconds, useconds;
  seconds = end.tv_sec - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  mtime = ((seconds)*1000 + useconds / 1000.0);

  free(buf);
  close_disk(fd);

  return mtime;
}

int open_disk() {
  int fd = open(disk_path, O_RDWR | O_CREAT | O_DIRECT, S_IRWXU);
  if (fd == -1) {
    perror("open");
    return -1;
  }
  return fd;
}

int close_disk(int fd) {
  if (close(fd) == -1) {
    perror("close");
    return -1;
  }
  return 0;
}