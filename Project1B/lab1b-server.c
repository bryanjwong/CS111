/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    lab1b-server.c
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
#include "zlib.h"

#define CHUNK 16384

z_stream sendstream, receivestream;

// Handle exiting program in one of 3 cases: closing pipe from client,
//  receiving a SIGPIPE from client, receiving an EOF from shell reader
void
handle_exit() {
  int wstatus;
  wait(&wstatus);
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\r\n", (wstatus & 0x007f), ((wstatus & 0xff00) >> 8));
  inflateEnd(&receivestream);
  deflateEnd(&sendstream);
  exit(0);
}

int
main(int argc, char *argv[]) {

  // Parse possible --shell option
  struct option long_options[] = {
    {"shell", required_argument,  0, 's'},
    {"port",  required_argument,  0, 'p'},
    {"compress", no_argument,     0, 'c'},
    {0, 0, 0, 0}
  };
  int opt_code;
  char * shell = NULL;
  char * port = NULL;
  char compress_flag = 0;
  while ((opt_code = getopt_long(argc, argv, "", long_options, 0)) != -1) {
    switch(opt_code) {
      case 's':
        shell = optarg;
        break;
      case 'p':
        port = optarg;
        break;
      case 'c':
        compress_flag = 1;

        sendstream.zalloc = Z_NULL;
        sendstream.zfree = Z_NULL;
        sendstream.opaque = Z_NULL;

        receivestream.zalloc = Z_NULL;
        receivestream.zfree = Z_NULL;
        receivestream.opaque = Z_NULL;

        if (inflateInit(&receivestream) != Z_OK) {
          fprintf(stderr, "Error while calling inflateInit\n");
          exit(1);
        }
        if (deflateInit(&sendstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
          fprintf(stderr, "Error while calling deflateInit\n");
          exit(1);
        }
        break;
      default:
        fprintf(stderr, "Usage: ./lab1b-server --port=portnum [--shell=program] [--compress]\n");
        exit(1);
    }
  }

  // Check that mandatory port option is specified
  if(!port) {
    fprintf(stderr, "Error, no --port argument specified\n");
    fprintf(stderr, "Usage: ./lab1b-server --port=portnum [--shell=program] [--compress]\n");
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
            handle_exit();
          }
          if (n == 0) {
            close(termtoshell_fd[1]);
          }

          char decompressed[CHUNK];
          if (compress_flag) {
            receivestream.avail_in = (uInt) n;
            receivestream.next_in = (Bytef *) c;
            receivestream.avail_out = (uInt) sizeof(decompressed);
            receivestream.next_out = (Bytef *) decompressed;

            while(receivestream.avail_in > 0)
              inflate(&receivestream, Z_SYNC_FLUSH);
            n = sizeof(decompressed) - receivestream.avail_out;
          }

          // Write input to shell
          for (ssize_t i = 0; i < n; i++) {
            char h = compress_flag ? decompressed[i] : c[i];
            switch(h) {
              case '\003':
                if (kill(cpid, SIGINT) < 0)
                  fprintf(stderr, "Killing child process failed: %s\r\n", strerror(errno));
                break;
              case '\004':
                close(termtoshell_fd[1]);
                break;
              default:
                write(termtoshell_fd[1], &h, 1);
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
            handle_exit();
          }
          char compressed[256];
          if (compress_flag) {
            sendstream.avail_in = (uInt) n;
            sendstream.next_in = (Bytef *) c;
            sendstream.avail_out = (uInt) sizeof(compressed);
            sendstream.next_out = (Bytef *) compressed;

            while(sendstream.avail_in > 0)
              deflate(&sendstream, Z_SYNC_FLUSH);
            n = sizeof(compressed) - sendstream.avail_out;
            write(newsockfd, compressed, n);
          }
          else {
            write(newsockfd, c, n);
          }

        }

        if (fds[1].revents & POLLHUP) {
          handle_exit();
        }
        if (fds[0].revents & POLLHUP) {
          fprintf(stderr, "Hang up from client: %s\r\n", strerror(errno));
          handle_exit();
        }
        if (fds[0].revents & POLLERR) {
          fprintf(stderr, "Error from client: %s\r\n", strerror(errno));
          handle_exit();
        }
        if (fds[1].revents & POLLERR) {
          fprintf(stderr, "Error from shell: %s\r\n", strerror(errno));
          handle_exit();
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
      write(newsockfd, c, n);

      char decompressed[CHUNK];
      ssize_t decompress_size;
      if (compress_flag) {
        receivestream.avail_in = (uInt) n;
        receivestream.next_in = (Bytef *) c;
        receivestream.avail_out = (uInt) sizeof(decompressed);
        receivestream.next_out = (Bytef *) decompressed;

        while(receivestream.avail_in > 0)
          inflate(&receivestream, Z_SYNC_FLUSH);
        decompress_size = sizeof(decompressed) - receivestream.avail_out;
      }
      // Echo input to stdout
      for (ssize_t i = 0; i < ((compress_flag) ? decompress_size : n); i++) {
        char h = compress_flag ? decompressed[i] : c[i];
        if(h == '\003' || h == '\004')
          exit(0);
      }
    }
    inflateEnd(&receivestream);
  }
  exit(0);
}
