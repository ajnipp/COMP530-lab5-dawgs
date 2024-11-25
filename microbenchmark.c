#include <stdio.h>
#include <stdlib.h>

void help() {
  printf("Usage: microbenchmark [options]\n");
  printf("Options:\n");
  printf("  -d <str>  disk path for both source and target\n");
}

int main(int argc, char **argv) {
  int numthreads = 1; // default to 1
  int c, i, rv;
  pthread_t *tinfo;
  long *opt_count;
  struct timeval start, end;
  long mtime, seconds, useconds;

  // Read options from command line:
  //   # clients from command line, as well as seed file
  //   Simulation length
  while ((c = getopt(argc, argv, "c:hl:s:")) != -1) {
    switch (c) {
    case 'c':
      break;
    case 'h':
      return 0;
    case 'l':
    //   simulation_length = atoi(optarg);
      break;
    case 's':
    //   global_salt = atoi(optarg);
      break;
    default:
      printf("Unknown option\n");
      help();
      return 1;
    }
  }
}