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
#include <getopt.h>
#include <poll.h>
#include <signal.h>

struct termios initial_termios_p;

// Reset terminal settings on exit
void
reset_terminal(void) {
  if(tcsetattr(0, TCSANOW, &initial_termios_p) == -1) {
    exit(1);
  }
}

// Disable echo and enable noncanonical input
void
configure_terminal(void) {
  struct termios my_termios_p;
  if(tcgetattr(0, &initial_termios_p) == -1) {
    fprintf(stderr, "Error, getting terminal failed: %s\n", strerror(errno));
    exit(1);
  }
  atexit(reset_terminal);
  if(tcgetattr(0, &my_termios_p) == -1) {
    fprintf(stderr, "Error, getting terminal failed: %s\n", strerror(errno));
    exit(1);
  }
  my_termios_p.c_iflag = ISTRIP;	// Only lower 7 bits
  my_termios_p.c_oflag = 0;
  my_termios_p.c_lflag = 0;
  if(tcsetattr(0, TCSANOW, &my_termios_p) == -1) {
    fprintf(stderr, "Error, terminal could not be set: %s\r\n", strerror(errno));
    exit(1);
  }
}

// Handle output to monitor
void
handle_echo(char c) {
  int n;
  if (c == '\003') {
    n = write(1, "^C\r\n", 4);
  }
  if (c == '\004') {
    n = write(1, "^D\r\n", 2);
  }
  else if ((c == '\r') || (c == '\n')) {
    n = write(1, "\r", 1);
    n = write(1, "\n", 1);
  }
  else {
    n = write(1, &c, 1);
  }
  if (n < 0) {
    fprintf(stderr, "Write to file descriptor 1 failed: %s\n", strerror(errno));
    exit(3);
  }
}

// Handle exiting program in one of 3 cases: closing pipe from keyboard reader,
//  receiving a SIGPIPE from keyboard reader, receiving an EOF from shell reader
void
handle_exit() {
  int wstatus;
  wait(&wstatus);
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\r\n", (wstatus & 0x007f), (wstatus & 0xff00));
  exit(0);
}

int
main(int argc, char *argv[]) {

  // Parse possible --shell option
  struct option long_options[] = {
    {"shell", required_argument,  0, 's'},
    {0, 0, 0, 0}
  };
  int opt_code;
  char * shell = NULL;
  while ((opt_code = getopt_long(argc, argv, "", long_options, 0)) != -1) {
    if (opt_code == 's') {
      shell = optarg;
    }
    else {
      fprintf(stderr, "Usage: ./lab1a [--shell=program]\n");
      exit(1);
    }
  }

  // Set up terminal to non-canonical input and no echo
  configure_terminal();

  // If shell option is specified, fork a new process
  int termtoshell_fd[2];
  int shelltoterm_fd[2];
  if (shell) {
    if ((pipe(termtoshell_fd) == -1) || (pipe(shelltoterm_fd) == -1)) {
      fprintf(stderr, "Pipe creation failed: %s\r\n", strerror(errno));
      exit(1);
    }

    pid_t cpid = fork();
    if (cpid == -1) {
      fprintf(stderr, "Forking process failed: %s\r\n", strerror(errno));
      exit(1);
    }

    /* Child process */
    if (cpid == 0) {

      //Replace stdin with the pipe
      close(0);
      dup(termtoshell_fd[0]);
      close(termtoshell_fd[0]);
      close(termtoshell_fd[1]);

      //Replace stdout/stderr with the pipe
      close(1);
      close(2);
      dup(shelltoterm_fd[1]);
      dup(shelltoterm_fd[1]);
      close(shelltoterm_fd[0]);
      close(shelltoterm_fd[1]);

      //Execute program in argument
      char * argv_list[] = {shell, NULL};
      if (execv(shell, argv_list) == -1) {
        fprintf(stderr, "Failed to execute %s: %s\r\n", shell, strerror(errno));
        exit(1);
      }
    }

    /* Parent Process */
    close(termtoshell_fd[0]);
    close(shelltoterm_fd[1]);

    // Register SIGPIPE handler
    if(signal(SIGPIPE, handle_exit) == SIG_ERR) {
      fprintf(stderr, "Error registering SIGPIPE handler: %s\r\n", strerror(errno));
    }

    // Set up polling
    struct pollfd fds[2];
    fds[0].fd = 0;
    fds[1].fd = shelltoterm_fd[0];
    fds[0].events = POLLIN | POLLHUP | POLLERR;
    fds[1].events = POLLIN | POLLHUP | POLLERR;

    char c[256];
    int n;
    while(1) {
      int ret = poll(fds, 2, 0);
      if (ret < 0) {
        fprintf(stderr, "Error while polling: %s\r\n", strerror(errno));
        exit(1);
      }
      if (ret > 0) {

        // Check keyboard input
        if (fds[0].revents & POLLIN) {
          // Handle keyboard input
          n = read(fds[0].fd, &c, 256);
          if (n < 0) {
            fprintf(stderr, "Read from keyboard input failed: %s\r\n", strerror(errno));
            exit(2);
          }
          // Echo input to stdout
          for (ssize_t i = 0; i < n; i++) {
            handle_echo(c[i]);
            switch(c[i]) {
              case '\003':
                if (kill(cpid, SIGINT) < 0)
                  fprintf(stderr, "Killing child process failed: %s\r\n", strerror(errno));
                break;
              case '\004':
                close(termtoshell_fd[1]);
                break;
              case '\r':
                write(termtoshell_fd[1], "\n", 1);
                break;
              default:
                write(termtoshell_fd[1], c+i, 1);
                break;
            }
          }
        }

        // Check input from shell
        if (fds[1].revents & POLLIN) {
          // Handle shell input
          n = read(fds[1].fd, &c, 256);
          if (n < 0) {
            fprintf(stderr, "Read from keyboard input failed: %s\r\n", strerror(errno));
            exit(2);
          }
          // EOF Reached
          if (n == 0) {
            close(shelltoterm_fd[0]);
            handle_exit();
          }
          // Write shell output to stdout
          for (ssize_t i = 0; i < n; i++) {
            handle_echo(c[i]);
          }
        }

        if (fds[1].revents & POLLHUP) {
          handle_exit();
        }
        if (fds[0].revents & POLLHUP) {
          fprintf(stderr, "Hang up from keyboard input: %s\r\n", strerror(errno));
          exit(1);
        }
        if (fds[0].revents & POLLERR) {
          fprintf(stderr, "Error from keyboard input: %s\r\n", strerror(errno));
          exit(1);
        }
        if (fds[1].revents & POLLERR) {
          fprintf(stderr, "Error from shell: %s\r\n", strerror(errno));
          exit(1);
        }
      }
    }
  }

  // No shell option specified
  else {
    char c[256];
    for (ssize_t n = read(0, &c, 256); n != 0; n = read(0, &c, 256)) {
      // Check for read Error
      if (n < 0) {
        fprintf(stderr, "Read from file descriptor 0 failed: %s\n", strerror(errno));
        exit(2);
      }
      // Echo input to stdout
      for (ssize_t i = 0; i < n; i++) {
        handle_echo(c[i]);
        if(c[i] == '\003' || c[i] == '\004')
          exit(0);
      }
    }
  }
  exit(0);
}
