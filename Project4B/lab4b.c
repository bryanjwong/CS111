/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    lab4b.c
***/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

int main(int argc, char *argv[]) {
  /* Parse possible --threads and --iterations options */
  struct option long_options[] = {
    {"period",      required_argument,  0,  'p'}, // Sampling interval in seconds
    {"scale",       required_argument,  0,  's'}, // Fahrenheit or Celsius
    {"log",         required_argument,  0,  'l'}, // Append reports to logfile
    {0, 0, 0, 0}
  };
  int opt_code;
  while ((opt_code = getopt_long(argc, argv, "", long_options, 0)) != -1) {
    switch (opt_code) {
      case 't':
        break;
      default:
        fprintf(stderr, "Usage: ./lab2_list [--threads=#] [--iterations=#] [--yield=idl] [--sync=m/s]\n");
        exit(1);
    }
  }
