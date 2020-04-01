/***
    lab0.c
    Author: Bryan Wong
    ID: 805111517

    Copy standard input to standard output using system calls
***/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>

void segfault() {
  char * p = NULL;
  *p = ' ';
}

void handleseg() {
  fprintf(stderr, "Error: caught segfault caused by --segfault option\n");
  exit(4);
}

int main(int argc, char *argv[]) {

  // Option List
  static struct option long_options[] = {
    {"input",     required_argument,  0, 'i'},
    {"output",    required_argument,  0, 'o'},
    {"segfault",  no_argument,        0, 's'},
    {"catch",     no_argument,        0, 'c'},
    {0, 0, 0, 0}
  };

  int opt_code;
  char segfault_flag = 0, catch_flag = 0;
  while ((opt_code = getopt_long(argc, argv, "", long_options, 0)) != -1) {

    int fd;
    switch(opt_code) {
      case 'i':
        fd = open(optarg, O_RDONLY);
        if (fd < 0) {
          fprintf(stderr, "Error with --input, could not open %s: %s\n", optarg, strerror(errno));
          exit(2);
        }
        if (close(0) < 0) {
          fprintf(stderr, "Error with --input, could not close stdin: %s\n", strerror(errno));
          exit(2);
        }
        if (dup(fd) < 0) {
          fprintf(stderr, "Error with --input, could not duplicate %s: %s\n", optarg, strerror(errno));
          exit(2);
        }
        if (close(fd) < 0) {
          fprintf(stderr, "Error with --input, could not close %s: %s\n", optarg, strerror(errno));
          exit(2);
        }
        break;

      case 'o':
        fd = creat(optarg, 0666);
        if (fd < 0) {
          fprintf(stderr, "Error with --output, could not creat %s: %s\n", optarg, strerror(errno));
          exit(3);
        }
        if (close(1) < 0) {
          fprintf(stderr, "Error with --output, could not close stdout: %s\n", strerror(errno));
          exit(3);
        }
        if (dup(fd) < 0) {
          fprintf(stderr, "Error with --output, could not duplicate %s: %s\n", optarg, strerror(errno));
          exit(3);
        };
        if (close(fd) < 0) {
          fprintf(stderr, "Error with --output, could not close %s: %s\n", optarg, strerror(errno));
          exit(3);
        }
        break;

      case 's':
        segfault_flag = 1;
        break;

      case 'c':
        catch_flag = 1;
        break;

      default:
        fprintf(stderr, "Usage: ./lab0 [--input=file] [--output=file] [--segfault] [--catch]\n");
        exit(1);
    }
  }

  if(catch_flag) {
    if(signal(SIGSEGV, handleseg) == SIG_ERR) {
      fprintf(stderr, "Error with --catch: %s\n", strerror(errno));
    }
  }

  if(segfault_flag) {
    segfault();
  }

  char c[1];
  for(ssize_t n = read(0, &c, 1); n != 0; n = read(0, &c, 1)) {
    // Check for read Error
    if (n < 0) {
      fprintf(stderr, "Read from file descriptor 0 failed: %s\n", strerror(errno));
      exit(2);
    }

    // Write to stdout
    n = write(1, &c, 1);
    if (n < 0) {
      fprintf(stderr, "Write to file descriptor 1 failed: %s\n", strerror(errno));
      exit(3);
    }
  }
  exit(0);
}
