/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    lab2_add.c
***/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <time.h>

long long counter = 0;
int opt_yield;
pthread_mutex_t mutexcount;
char spinlock = 0;
void (*addfunc_ptr) (long long *, long long);

/* Add function as given by spec */
void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
}

/* Add function with mutexes */
void add_m(long long *pointer, long long value) {
  pthread_mutex_lock(&mutexcount);
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
  pthread_mutex_unlock(&mutexcount);
}

/* Add function with spin lock */
void add_s(long long *pointer, long long value) {
  while(__sync_lock_test_and_set(&spinlock, 1) == 1);
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
  __sync_lock_release(&spinlock);
}

/* Add function with atomic compare and swap */
void add_c(long long *pointer, long long value) {
  long long initial_val;
  long long sum;
  do {
    initial_val = *pointer;
    sum = initial_val + value;
    if (opt_yield)
      sched_yield();
  } while(__sync_val_compare_and_swap(pointer, initial_val, sum) != initial_val);
}

/* Increment, then decrement for a specified number of times */
void *incdec(void *niters) {
  long niterations = (long)niters;
  for(long i = 0; i < niterations; i++) {
    (*addfunc_ptr)(&counter, 1);
  }
  for(long i = 0; i < niterations; i++) {
    (*addfunc_ptr)(&counter, -1);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  /* Parse possible --threads and --iterations options */
  struct option long_options[] = {
    {"threads",     optional_argument,  0,  't'},
    {"iterations",  optional_argument,  0,  'i'},
    {"yield",       no_argument,        0,  'y'},
    {"sync",        required_argument,  0,  's'},
    {0, 0, 0, 0}
  };
  long nthreads = 1;
  long niterations = 1;
  char sync;
  int opt_code;
  addfunc_ptr = &add;
  while ((opt_code = getopt_long(argc, argv, "", long_options, 0)) != -1) {
    switch (opt_code) {
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
      case 'y':
        opt_yield = 1;
        break;
      case 's':
        if (strlen(optarg) != 1) {
          fprintf(stderr, "Invalid --sync argument: %s\n", optarg);
          fprintf(stderr, "Usage: ./lab2_add [--threads=#] [--iterations=#]\n");
          exit(1);
        }
        sync = *optarg;
        switch (sync) {
          case 'm':
            if (pthread_mutex_init(&mutexcount, NULL) != 0) {
              fprintf(stderr, "Unable to create mutex.\n");
              exit(1);
            }
            addfunc_ptr = &add_m;
            break;
          case 's':
            addfunc_ptr = &add_s;
            break;
          case 'c':
            addfunc_ptr = &add_c;
            break;
          default:
            fprintf(stderr, "Invalid --sync argument: %s\n", optarg);
            fprintf(stderr, "Usage: ./lab2_add [--threads=#] [--iterations=#]\n");
            exit(1);
        }
        break;
      default:
        fprintf(stderr, "Usage: ./lab2_add [--threads=#] [--iterations=#] [--yield] [--sync=m/s/c]\n");
        exit(1);
    }
  }

  pthread_t * threads = malloc(sizeof(pthread_t) * nthreads);
  if (threads == NULL) {
    fprintf(stderr, "Dynamic allocation of threads failed: %s\n", strerror(errno));
    exit(1);
  }
  pthread_attr_t attr;
  int rc;
  struct timespec ts_old;
  struct timespec ts_new;

  /* Explicitly state threads are joinable */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  /* Capture high resolution starting time */
  if (clock_gettime(CLOCK_MONOTONIC, &ts_old) == -1) {
    fprintf(stderr, "Pre-operation clock time retrieval failed: %s\n", strerror(errno));
    free(threads);
    exit(1);
  }

  /* Call increment/decrement routine for each thread */
  for (long t = 0; t < nthreads; t++) {
     rc = pthread_create(&threads[t], &attr, incdec, (void *)niterations);
     if (rc) {
       fprintf(stderr, "Error, return code from pthread_create() is %d\n", rc);
       free(threads);
       exit(2);
     }
  }

  /* Join all threads */
  pthread_attr_destroy(&attr);
  for (long t = 0; t < nthreads; t++) {
    rc = pthread_join(threads[t], NULL);
    if (rc) {
      fprintf(stderr, "Error, return code from pthread_join() is %d\n", rc);
      free(threads);
      exit(2);
    }
  }

  /* Capture post-operation time */
  if (clock_gettime(CLOCK_MONOTONIC, &ts_new) == -1) {
    fprintf(stderr, "Post-operation clock time retrieval failed: %s\n", strerror(errno));
    free(threads);
    exit(1);
  }

  /* Perform exit operations */
  if (sync == 'm')
      pthread_mutex_destroy(&mutexcount);

  /* Print CSV Output */
  long long s_elapsed = (long long) ts_new.tv_sec - ts_old.tv_sec;
  long long ns_elapsed = s_elapsed * 1000000000 + (ts_new.tv_nsec - ts_old.tv_nsec);
  long noperations = nthreads * niterations * 2;

  fprintf(stdout, "add-");
  if (opt_yield) {
    fprintf(stdout, "yield-");
  }
  if (sync) {
    fprintf(stdout, "%c,", sync);
  } else {
    fprintf(stdout, "none,");
  }

  fprintf(stdout, "%ld,%ld,%ld,%lld,%lld,%lld\n", nthreads, niterations,
          noperations, ns_elapsed, ns_elapsed/noperations, counter);
  free(threads);
  exit(0);
}
