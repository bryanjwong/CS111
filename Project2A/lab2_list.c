/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    lab2_list.c
***/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "SortedList.h"

#define KEY_SIZE 64

/* Struct used to pass data to thread routine */
typedef struct t_data {
  long tid;
  long niterations;
  SortedListElement_t *elements;
  SortedList_t *head;
} t_data;

void *list_test(void *t) {
  t_data *data = (t_data *)t;
  for (long i = 0; i < data->niterations; i++) {
    long index = data->tid * data->niterations + i;
    // fprintf(stdout, "KEY #%ld: %s (Thread %ld)\n", index, data->elements[index].key, data->tid);
    SortedList_insert(data->head, &(data->elements[index]));
  }
  int length = SortedList_length(data->head);
  if (length == -1) {
    fprintf(stderr, "Synchronization error detected: list length could not be found\n");
    exit(2);
  }
  for (long i = 0; i < data->niterations; i++) {
    long index = data->tid * data->niterations + i;
    SortedListElement_t *del = SortedList_lookup(data->head, data->elements[index].key);
    if (del) {
      if (SortedList_delete(del)) {
        fprintf(stderr, "Synchronization error detected: element deletion failed\n");
        exit(2);
      }
    }
    else {
      fprintf(stderr, "Synchronization error detected: inserted keys cannot be found\n");
      exit(2);
    }
  }
  pthread_exit(NULL);
}

void handleseg() {
  fprintf(stderr, "Synchronization error detected: segfault caught\n");
  exit(2);
}

int main(int argc, char *argv[]) {
  /* Parse possible --threads and --iterations options */
  struct option long_options[] = {
    {"threads",     optional_argument,  0,  't'},
    {"iterations",  optional_argument,  0,  'i'},
    {"yield",       required_argument,  0,  'y'},
    {0, 0, 0, 0}
  };
  long nthreads = 1;
  long niterations = 1;
  int opt_code;
  char yield_flag = 0;
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
        if (optarg) {
          for (int i = 0; optarg[i] != '\0'; i++) {
            yield_flag = 1;
            switch (optarg[i]) {
              case 'i':
                opt_yield |= INSERT_YIELD;
                break;
              case 'd':
                opt_yield |= DELETE_YIELD;
                break;
              case 'l':
                opt_yield |= LOOKUP_YIELD;
                break;
              default:
                fprintf(stderr, "Invalid --yield argument: %c\n", optarg[i]);
                exit(1);
            }
          }
        }
        break;
      default:
        fprintf(stderr, "Usage: ./lab2a_add [--threads=#] [--iterations=#] [--yield=idl]\n");
        exit(1);
    }
  }

  long nelements = nthreads * niterations;
  char ** keys = malloc(nelements * sizeof(char *));
  if (keys == NULL) {
    fprintf(stderr, "Dynamic allocation of keys failed: %s\n", strerror(errno));
    exit(1);
  }
  SortedListElement_t * elements = malloc(nelements * sizeof(SortedListElement_t));
  if (elements == NULL) {
    fprintf(stderr, "Dynamic allocation of list elements failed: %s\n", strerror(errno));
    exit(1);
  }

  /* Each key is a randomly generated string of 64 chars A-Z */
  for (int i = 0; i < nelements; i++) {
    keys[i] = malloc((KEY_SIZE + 1) * sizeof(char));
    if (keys[i] == NULL) {
      fprintf(stderr, "Dynamic allocation of key #%d failed: %s\n", i, strerror(errno));
      exit(1);
    }
    for (int j = 0; j < KEY_SIZE; j++) {
      keys[i][j] = 'A' + (rand() % 26);
    }
    keys[i][KEY_SIZE] = '\0';
    elements[i].key = keys[i];
    // fprintf(stdout, "KEY #%d: %s\n", i, elements[i].key);
  }

  /* Initialize an empty list */
  SortedList_t head;
  head.next = &head;
  head.prev = &head;
  head.key = NULL;

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

  /* Handle segfault signal catcher */
  if(signal(SIGSEGV, handleseg) == SIG_ERR) {
    fprintf(stderr, "Error registering SIGSEGV handler: %s\n", strerror(errno));
  }

  /* Capture high resolution starting time */
  if (clock_gettime(CLOCK_MONOTONIC, &ts_old) == -1) {
    fprintf(stderr, "Pre-operation clock time retrieval failed: %s\n", strerror(errno));
    exit(1);
  }

  t_data *data = malloc(nthreads * sizeof(t_data));
  /* Call list test routine for each thread */
  for (long t = 0; t < nthreads; t++) {
    data[t].tid = t;
    data[t].niterations = niterations;
    data[t].elements = elements;
    data[t].head = &head;
    rc = pthread_create(&threads[t], &attr, list_test, (void *) &data[t]);
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
      exit(2);
    }
  }

  /* Capture post-operation time */
  if (clock_gettime(CLOCK_MONOTONIC, &ts_new) == -1) {
    fprintf(stderr, "Post-operation clock time retrieval failed: %s\n", strerror(errno));
    exit(1);
  }

  int final_length = SortedList_length(&head);
  if (final_length != 0) {
    fprintf(stderr, "Synchronization error detected: final list length is non-zero\n");
    exit(2);
  }

  /* Print CSV Output */
  long long s_elapsed = (long long) ts_new.tv_sec - ts_old.tv_sec;
  long long ns_elapsed = s_elapsed * 1000000000 + (ts_new.tv_nsec - ts_old.tv_nsec);
  long noperations = nthreads * niterations * 3;

  char yieldopts[] = "none";
  if (yield_flag) {
    int index = 0;
    if (opt_yield & INSERT_YIELD) {
      yieldopts[index] = 'i';
      index++;
    }
    if (opt_yield & DELETE_YIELD) {
      yieldopts[index] = 'd';
      index++;
    }
    if (opt_yield & LOOKUP_YIELD) {
      yieldopts[index] = 'l';
      index++;
    }
    yieldopts[index] = '\0';
  }
  char *syncopts = "none";

  fprintf(stdout, "list-%s-%s,", yieldopts, syncopts);
  fprintf(stdout, "%ld,%ld,1,%ld,%lld,%lld\n", nthreads, niterations,
          noperations, ns_elapsed, ns_elapsed/noperations);

  /* Free dynamically allocated variables */
  for (int i = 0; i < nelements; i++) {
    free(keys[i]);
  }
  free(keys);
  free(elements);
  free(threads);
  free(data);
  return 0;
}
