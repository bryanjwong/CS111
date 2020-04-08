/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    lab1.c
    Copy standard input to standard output using system calls
***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

struct termios initial_termios_p;

void
reset_terminal(void) {
  tcsetattr(0, TCSANOW, &initial_termios_p);
}

int
main(int argc, char *argv[]) {

  // Argument Error Handling
  if(argc > 2) {
    fprintf(stderr, "Error, expected at most 1 argument but received %x\n", argc-1);
    exit(1);
  }
  if(argc == 2) {
    if(strcmp(argv[1], "--shell")) {
      fprintf(stderr, "Error, invalid argument received: %s\n", argv[1]);
      exit(1);
    }
    else {
      printf("Received --shell argument\n");
    }
  }

  struct termios my_termios_p;
  tcgetattr(0, &initial_termios_p);
  atexit(reset_terminal);

  printf("INITIAL VALUES\n");
  printf("==============\n");
  printf("%lu\n", initial_termios_p.c_iflag);
  printf("%lu\n", initial_termios_p.c_oflag);
  printf("%lu\n", initial_termios_p.c_cflag);
  printf("%lu\n", initial_termios_p.c_lflag);

  tcgetattr(0, &my_termios_p);
  my_termios_p.c_iflag = ISTRIP;	// Only lower 7 bits
	my_termios_p.c_oflag = 0;
	my_termios_p.c_lflag = 0;
  tcsetattr(0, TCSANOW, &my_termios_p);

  char c[1];
  for(ssize_t n = read(0, &c, 1); n != 0; n = read(0, &c, 1)) {
    // Check for read Error
    if (n < 0) {
      fprintf(stderr, "Read from file descriptor 0 failed: %s\n", strerror(errno));
      exit(2);
    }

  }
  write(1, &c, 1);

  exit(0);
}
