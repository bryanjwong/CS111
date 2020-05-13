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

char opt_sync = 0;
char * spinlock = NULL;
long nlists = 1;
pthread_mutex_t * mutexlist = NULL;

/* Struct used to pass data to thread routine */
typedef struct t_data {
  long tid;
  long niterations;
  SortedListElement_t *elements;
  SortedList_t *heads;
} t_data;

unsigned long hash(const char * key) {
  unsigned long hash = 5381;
  for (int i = 0; i < KEY_SIZE; i++) {
    hash = ((hash << 5) + hash) + key[i]; /* hash * 33 + key[i] */
  }
  return hash % nlists;
}

void *list_test(void *t) {
  struct timespec timer_start;
  struct timespec timer_fin;
  long long * thread_time = malloc(sizeof(long long));
  *thread_time = 0;
  t_data *data = (t_data *)t;
  for (long i = 0; i < data->niterations; i++) {
    long index = data->tid * data->niterations + i;
    unsigned long hashed_key = hash(data->elements[index].key);
    if (opt_sync) {
      /* Capture high resolution starting time */
      if (clock_gettime(CLOCK_MONOTONIC, &timer_start) == -1) {
        fprintf(stderr, "Pre-operation clock time retrieval failed: %s\n", strerror(errno));
        exit(1);
      }
    }
    if (opt_sync == 'm')
      pthread_mutex_lock(&mutexlist[hashed_key]);
    if (opt_sync == 's')
      while(__sync_lock_test_and_set(&spinlock[hashed_key], 1) == 1);
    if (opt_sync) {
      /* Capture high resolution finish time */
      if (clock_gettime(CLOCK_MONOTONIC, &timer_fin) == -1) {
        fprintf(stderr, "Post-operation clock time retrieval failed: %s\n", strerror(errno));
        exit(1);
      }
      long long s_elapsed = (long long) timer_fin.tv_sec - timer_start.tv_sec;
      long long ns_elapsed = s_elapsed * 1000000000 + (timer_fin.tv_nsec - timer_start.tv_nsec);
      (*thread_time) += ns_elapsed;
    }
    // fprintf(stdout, "KEY #%ld: %s (Thread %ld)\n", index, data->elements[index].key, data->tid);
    SortedList_insert(&data->heads[hashed_key], &(data->elements[index]));
    if (opt_sync == 'm')
      pthread_mutex_unlock(&mutexlist[hashed_key]);
    if (opt_sync == 's')
      __sync_lock_release(&spinlock[hashed_key]);
  }

  int length = 0;
  for (long i = 0; i < nlists; i++) {
    if (opt_sync) {
      /* Capture high resolution starting time */
      if (clock_gettime(CLOCK_MONOTONIC, &timer_start) == -1) {
        fprintf(stderr, "Pre-operation clock time retrieval failed: %s\n", strerror(errno));
        exit(1);
      }
    }
    if (opt_sync == 'm')
      pthread_mutex_lock(&mutexlist[i]);
    if (opt_sync == 's')
      while(__sync_lock_test_and_set(&spinlock[i], 1) == 1);
    if (opt_sync) {
      /* Capture high resolution finish time */
      if (clock_gettime(CLOCK_MONOTONIC, &timer_fin) == -1) {
        fprintf(stderr, "Post-operation clock time retrieval failed: %s\n", strerror(errno));
        exit(1);
      }
      long long s_elapsed = (long long) timer_fin.tv_sec - timer_start.tv_sec;
      long long ns_elapsed = s_elapsed * 1000000000 + (timer_fin.tv_nsec - timer_start.tv_nsec);
      (*thread_time) += ns_elapsed;
    }
    int rc = SortedList_length(&data->heads[i]);
    if (rc == -1) {
      fprintf(stderr, "Synchronization error detected: list length could not be found\n");
      exit(2);
    }
    length += rc;
    if (opt_sync == 'm')
      pthread_mutex_unlock(&mutexlist[i]);
    if (opt_sync == 's')
      __sync_lock_release(&spinlock[i]);
    if (opt_sync) {
      /* Capture high resolution finish time */
      if (clock_gettime(CLOCK_MONOTONIC, &timer_fin) == -1) {
        fprintf(stderr, "Post-operation clock time retrieval failed: %s\n", strerror(errno));
        exit(1);
      }
      long long s_elapsed = (long long) timer_fin.tv_sec - timer_start.tv_sec;
      long long ns_elapsed = s_elapsed * 1000000000 + (timer_fin.tv_nsec - timer_start.tv_nsec);
      (*thread_time) += ns_elapsed;
    }
  }

  for (long i = 0; i < data->niterations; i++) {
    long index = data->tid * data->niterations + i;
    unsigned long hashed_key =  hash(data->elements[index].key);
    if (opt_sync) {
      /* Capture high resolution starting time */
      if (clock_gettime(CLOCK_MONOTONIC, &timer_start) == -1) {
        fprintf(stderr, "Pre-operation clock time retrieval failed: %s\n", strerror(errno));
        exit(1);
      }
    }
    if (opt_sync == 'm')
      pthread_mutex_lock(&mutexlist[hashed_key]);
    if (opt_sync == 's')
      while(__sync_lock_test_and_set(&spinlock[hashed_key], 1) == 1);
    SortedListElement_t *del = SortedList_lookup(&data->heads[hashed_key], data->elements[index].key);
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
    if (opt_sync == 'm')
      pthread_mutex_unlock(&mutexlist[hashed_key]);
    if (opt_sync == 's')
      __sync_lock_release(&spinlock[hashed_key]);
  }
  pthread_exit((void *)thread_time);
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
    {"sync",        required_argument,  0,  's'},
    {"lists",       required_argument,  0,  'l'},
    {0, 0, 0, 0}
  };
  long nthreads = 1;
  long niterations = 1;
  char yield_flag = 0;
  char mutex_flag = 0;
  char spinlock_flag = 0;
  int opt_code;
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
      case 's':
        if (strlen(optarg) != 1) {
          fprintf(stderr, "Invalid --sync argument: %s\n", optarg);
          fprintf(stderr, "Usage: ./lab2_list [--threads=#] [--iterations=#] [--yield=idl] [--sync=m/s]\n");
          exit(1);
        }
        opt_sync = *optarg;
        switch (opt_sync) {
          case 'm':
            mutex_flag = 1;
            break;
          case 's':
            spinlock_flag = 1;
            break;
          default:
            fprintf(stderr, "Invalid --sync argument: %s\n", optarg);
            fprintf(stderr, "Usage: ./lab2_list [--threads=#] [--iterations=#] [--yield=idl] [--sync=m/s]\n");
            exit(1);
        }
        break;
      case 'l':
        if (optarg) {
          int lists = atoi(optarg);
          if (lists > 0) {
            nlists = lists;
          } else {
            fprintf(stderr, "Invalid number of lists: %d\n", lists);
            exit(1);
          }
        }
        break;
      default:
        fprintf(stderr, "Usage: ./lab2_list [--threads=#] [--iterations=#] [--yield=idl] [--sync=m/s]\n");
        exit(1);
    }
  }
  if (mutex_flag) {
    mutexlist = malloc(sizeof(pthread_mutex_t) * nlists);
    if (mutexlist == NULL) {
      fprintf(stderr, "Dynamic allocation of mutexes failed: %s\n", strerror(errno));
      exit(1);
    }
    for (long i = 0; i < nlists; i++) {
      if (pthread_mutex_init(&mutexlist[i], NULL) != 0) {
        fprintf(stderr, "Unable to create mutex.\n");
        exit(1);
      }
    }
  }
  if (spinlock_flag) {
    spinlock = calloc(nlists, sizeof(char));
    if (spinlock == NULL) {
      fprintf(stderr, "Dynamic allocation of spinlocks failed: %s\n", strerror(errno));
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
  SortedList_t * heads = malloc(sizeof(SortedList_t) * nlists);
  if (heads == NULL) {
    fprintf(stderr, "Dynamic allocation of threads failed: %s\n", strerror(errno));
    exit(1);
  }
  for (long i = 0; i < nlists; i++) {
    heads[i].next = &heads[i];
    heads[i].prev = &heads[i];
    heads[i].key = NULL;
  }

  pthread_t * threads = malloc(sizeof(pthread_t) * nthreads);
  if (threads == NULL) {
    fprintf(stderr, "Dynamic allocation of threads failed: %s\n", strerror(errno));
    exit(1);
  }
  int rc;
  struct timespec ts_old;
  struct timespec ts_new;
  void * return_val = NULL;
  long long lock_time = 0;

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
    data[t].heads = heads;
    rc = pthread_create(&threads[t], NULL, list_test, (void *) &data[t]);
    if (rc) {
      fprintf(stderr, "Error, return code from pthread_create() is %d\n", rc);
      free(threads);
      exit(2);
    }
  }

  /* Join all threads */
  for (long t = 0; t < nthreads; t++) {
    rc = pthread_join(threads[t], &return_val);
    if (rc) {
      fprintf(stderr, "Error, return code from pthread_join() is %d\n", rc);
      exit(2);
    }
    long long * thread_time = (long long *) return_val;
    lock_time += *thread_time;
    free(return_val);
  }
  if(!opt_sync)
    lock_time = 0;

  /* Capture post-operation time */
  if (clock_gettime(CLOCK_MONOTONIC, &ts_new) == -1) {
    fprintf(stderr, "Post-operation clock time retrieval failed: %s\n", strerror(errno));
    exit(1);
  }

  int final_length = 0;
  for (long i = 0; i < nlists; i++) {
    int rc = SortedList_length(&data->heads[i]);
    if (rc == -1) {
      fprintf(stderr, "Synchronization error detected: list length could not be found\n");
      exit(2);
    }
    final_length += rc;
  }
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

  fprintf(stdout, "list-%s-", yieldopts);
  if(opt_sync) {
    fprintf(stdout, "%c,", opt_sync);
  } else {
    fprintf(stdout, "%s,", "none");
  }
  fprintf(stdout, "%ld,%ld,%ld,%ld,%lld,%lld,%lld\n", nthreads, niterations, nlists,
          noperations, ns_elapsed, ns_elapsed/noperations, lock_time/noperations);

  /* Free dynamically allocated variables */
  if (opt_sync == 'm') {
    for(long i = 0; i < nlists; i++)
      pthread_mutex_destroy(&mutexlist[i]);
  }
  for (int i = 0; i < nelements; i++) {
    free(keys[i]);
  }
  free(keys);
  free(elements);
  free(threads);
  free(data);
  free(heads);
  free(mutexlist);
  free(spinlock);
  return 0;
}
