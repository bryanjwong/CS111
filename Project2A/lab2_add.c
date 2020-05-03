/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    lab1b-client.c
***/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <time.h>

long long counter = 0;


/* Add function as given by spec */
void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  *pointer = sum;
}

void *incdec(void *niters) {
  long niterations = (long)niters;
  for(long i = 0; i < niterations; i++) {
    add(&counter, 1);
  }
  for(long i = 0; i < niterations; i++) {
    add(&counter, -1);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  // Parse possible --threads and --iterations options
  struct option long_options[] = {
    {"threads",     optional_argument,  0,  't'},
    {"iterations",  optional_argument,  0,  'i'},
    {0, 0, 0, 0}
  };
  long nthreads = 1;
  long niterations = 1;
  int opt_code;
  while ((opt_code = getopt_long(argc, argv, "", long_options, 0)) != -1) {
    switch(opt_code) {
      case 't':
        if (optarg) {
          int threads = atoi(optarg);
          if (threads > 0) {
            nthreads = threads;
          } else {
            fprintf(stderr, "Invalid number of threads: %d\n", threads);
            exit(1);
          }
        }
        break;
      case 'i':
        if (optarg) {
          int iterations = atoi(optarg);
          if (iterations > 0) {
            niterations = iterations;
          } else {
            fprintf(stderr, "Invalid number of iterations: %d\n", iterations);
            exit(1);
          }
        }
        break;
      default:
        fprintf(stderr, "Usage: ./lab2a_add [--threads=#] [--iterations=#]\n");
        exit(1);
    }
  }

  pthread_t * threads = malloc(sizeof(pthread_t) * nthreads);
  pthread_attr_t attr;
  int rc;
  struct timespec ts_old;
  struct timespec ts_new;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  if (clock_gettime(CLOCK_MONOTONIC, &ts_old) == -1) {
    fprintf(stderr, "Pre-operation clock time retrieval failed: %s\r\n", strerror(errno));
    free(threads);
    exit(1);
  }

  for (long t = 0; t < nthreads; t++) {
     rc = pthread_create(&threads[t], &attr, incdec, (void *)niterations);
     if (rc) {
       fprintf(stderr, "Error, return code from pthread_create() is %d\n", rc);
       free(threads);
       exit(1);
     }
  }

  pthread_attr_destroy(&attr);
  for (long t = 0; t < nthreads; t++) {
    rc = pthread_join(threads[t], NULL);
    if (rc) {
      fprintf(stderr, "Error, return code from pthread_join() is %d\n", rc);
      free(threads);
      exit(1);
    }
  }

  if (clock_gettime(CLOCK_MONOTONIC, &ts_new) == -1) {
    fprintf(stderr, "Post-operation clock time retrieval failed: %s\r\n", strerror(errno));
    free(threads);
    exit(1);
  }

  long long s_elapsed = (long long) ts_new.tv_sec - ts_new.tv_sec;
  long long ns_elapsed = s_elapsed * 1000000000 + (ts_new.tv_nsec - ts_old.tv_nsec);
  long long noperations = nthreads * niterations * 2;

  fprintf(stdout, "add-none,%ld,%ld,%lld,%lld,%lld,%lld\n", nthreads, niterations,
          noperations, ns_elapsed, ns_elapsed/noperations, counter);
  free(threads);
}
