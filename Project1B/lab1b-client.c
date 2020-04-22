/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    lab1b.c
    Configure terminal to use non-canonical
    input that echoes user keyboard input to
    stdout. If --shell option is set, pass
    input to shell program and pass shell
    output to stdout.
***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <getopt.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>

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
    n = write(1, "^C", 2);
  }
  if (c == '\004') {
    n = write(1, "^D", 2);
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

int
main(int argc, char *argv[]) {

  // Parse possible --shell option
  struct option long_options[] = {
    {"port",  required_argument,  0, 'p'},
    {"log",   required_argument,  0, 'l'},
    {0, 0, 0, 0}
  };
  int opt_code;
  char * port = NULL;
  char * logfile = NULL;
  int logfd = 0;
  while ((opt_code = getopt_long(argc, argv, "", long_options, 0)) != -1) {
    if (opt_code == 'p') {
      port = optarg;
    }
    else if (opt_code == 'l') {
      logfile = optarg;
    }
    else {
      fprintf(stderr, "Usage: ./lab1b-client --port=portnum [--log=filename]\n");
      exit(1);
    }
  }

  // Check that mandatory port option is specified
  if (!port) {
    fprintf(stderr, "Error, no --port argument specified\n");
    fprintf(stderr, "Usage: ./lab1b-client --port=portnum [--log=filename]\n");
    exit(1);
  }

  if (logfile) {
    logfd = creat(logfile, 0666);
    if (logfd < 0) {
      fprintf(stderr, "Error with logfile, could not creat %s: %s\n", logfile, strerror(errno));
      exit(1);
    }
  }

  int sockfd, portno;

  struct sockaddr_in serv_addr;
  struct hostent *server;

  portno = atoi(port);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
     fprintf(stderr, "Error opening socket: %s", strerror(errno));
     exit(1);
  }
  server = gethostbyname("localhost");
  if (server == NULL) {
      fprintf(stderr,"Error, no such host\n");
      exit(1);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    fprintf(stderr,"Error while connecting: %s\n", strerror(errno));
    exit(1);
  }

  // Set up terminal to non-canonical input and no echo
  configure_terminal();

  // Set up polling
  struct pollfd fds[2];
  fds[0].fd = 0;
  fds[1].fd = sockfd;
  fds[0].events = POLLIN | POLLHUP | POLLERR;
  fds[1].events = POLLIN | POLLHUP | POLLERR;

  char c[256];
  ssize_t n;

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
        n = read(fds[0].fd, &c, 255);
        if (n < 0) {
          fprintf(stderr, "Read from stdin failed: %s\r\n", strerror(errno));
          exit(1);
        }

        if (logfd) {
          char numbytes[21];
          int numbytes_size = sprintf(numbytes, "%zd", n);
          if (numbytes_size > 0) {
            if (write(logfd, "SENT ", 5) < 0) {
              fprintf(stderr, "Write to log file failed: %s\r\n", strerror(errno));
              exit(1);
            }
            write(logfd, numbytes, numbytes_size);
            write(logfd, " bytes: ", 8);
          }
        }
        // Write input to socket
        for (ssize_t i = 0; i < n; i++) {
          handle_echo(c[i]);
          n = write(sockfd, c+i, 1);
          if (n < 0) {
            fprintf(stderr, "Write to socket failed: %s\r\n", strerror(errno));
            exit(1);
          }

          if (logfd) {
            switch(c[i]) {
              case '\003':
                write(logfd, "^C", 2);
                break;
              case '\004':
                write(logfd, "^D", 2);
                break;
              case '\r':
                write(logfd, "\n", 1);
                break;
              default:
                write(logfd, c+i, 1);
                break;
            }
          }
        }
        if (logfd) {
          write(logfd, "\n", 1);
        }
      }

      // Check input from socket
      if (fds[1].revents & POLLIN) {
        // Handle shell input
        n = read(fds[1].fd, &c, 256);
        if (n < 0) {
          fprintf(stderr, "Read from socket input failed: %s\r\n", strerror(errno));
          exit(2);
        }
        // Write socket output to stdout
        for (ssize_t i = 0; i < n; i++) {
          handle_echo(c[i]);
        }

        if (logfd) {
          char numbytes[21];
          int numbytes_size = sprintf(numbytes, "%zd", n);
          if (n > 0) {
            write(logfd, "RECEIVED ", 9);
            write(logfd, numbytes, numbytes_size);
            write(logfd, " bytes: ", 8);
            write(logfd, c, n);
            write(logfd, "\n", 1);
          }
        }
      }

      if (fds[1].revents & POLLHUP) {
        fprintf(stderr, "Hang up from socket: %s\r\n", strerror(errno));
        exit(0);
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
        fprintf(stderr, "Error from socket: %s\r\n", strerror(errno));
        exit(1);
      }
    }
  }
}
