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
#include <errno.h>
#include <getopt.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\r\n", (wstatus & 0x007f), ((wstatus & 0xff00) >> 8));
  exit(0);
}

int
main(int argc, char *argv[]) {

  // Parse possible --shell option
  struct option long_options[] = {
    {"shell", required_argument,  0, 's'},
    {"port",  required_argument,  0, 'p'},
    {0, 0, 0, 0}
  };
  int opt_code;
  char * shell = NULL;
  char * port = NULL;
  while ((opt_code = getopt_long(argc, argv, "", long_options, 0)) != -1) {
    if (opt_code == 's') {
      shell = optarg;
    }
    else if (opt_code == 'p') {
      port = optarg;
    }
    else {
      fprintf(stderr, "Usage: ./lab1b-server --port=portnum [--shell=program]\n");
      exit(1);
    }
  }

  // Check that mandatory port option is specified
  if(!port) {
    fprintf(stderr, "Error, no --port argument specified\n");
    fprintf(stderr, "Usage: ./lab1b-server --port=portnum [--shell=program]\n");
    exit(1);
  }

  // Set up server to listen at specified port
  int sockfd, newsockfd, portno, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
     fprintf(stderr, "Error opening socket: %s", strerror(errno));
     exit(1);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(port);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "Error on binding socket: %s", strerror(errno));
    exit(1);
  }
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
  if (newsockfd < 0) {
    fprintf(stderr, "Error on accept: %s", strerror(errno));
    exit(1);
  }

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
    fds[0].fd = newsockfd;
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

        // Check socket input
        if (fds[0].revents & POLLIN) {
          // Handle socket input
          n = read(fds[0].fd, &c, 255);
          if (n < 0) {
            fprintf(stderr, "Read from socket failed: %s\r\n", strerror(errno));
            close(termtoshell_fd[1]);
          }
          if (n == 0) {
            close(termtoshell_fd[1]);
          }
          // Write input to shell
          for (ssize_t i = 0; i < n; i++) {
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
            fprintf(stderr, "Read from shell input failed: %s\r\n", strerror(errno));
            exit(1);
          }
          // EOF Reached
          if (n == 0) {
            close(shelltoterm_fd[0]);
            write(1, "EOF\n", 4);
            handle_exit();
          }
          // Write shell output to socket
          if (write(newsockfd, c, n) < 0) {
            fprintf(stderr, "Write to socket failed: %s\r\n", strerror(errno));
            exit(1);
          }
        }

        if (fds[1].revents & POLLHUP) {
          handle_exit();
        }
        if (fds[0].revents & POLLHUP) {
          fprintf(stderr, "Hang up from client: %s\r\n", strerror(errno));
          exit(1);
        }
        if (fds[0].revents & POLLERR) {
          fprintf(stderr, "Error from client: %s\r\n", strerror(errno));
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
    for (ssize_t n = read(newsockfd, &c, 255); n != 0; n = read(newsockfd, &c, 255)) {
      // Check for read Error
      if (n < 0) {
        fprintf(stderr, "Read from socket failed: %s\n", strerror(errno));
        exit(1);
      }
      // Echo input to stdout
      for (ssize_t i = 0; i < n; i++) {
        write(newsockfd, c+i, 1);

        // TODO: RESOLVE THIS
        if(c[i] == '\003' || c[i] == '\004')
          exit(0);
      }
    }
  }
  exit(0);
}
