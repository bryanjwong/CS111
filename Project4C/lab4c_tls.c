/***
    NAME: Bryan Wong
    EMAIL: bryanjwong@g.ucla.edu
    ID: 805111517

    lab4c.c
***/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <mraa.h>
#include <math.h>
#include <time.h>
#include <poll.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <netdb.h>

#define TEMPERATURE_MRAA 1

mraa_aio_context temperature;
struct tm last_time;

SSL * ssl;

/* Default Settings */
int SAMPLE_PERIOD = 1;
char SCALE = 'F';
char RUN_FLAG = 1;
int logfd = 0;
char * command = NULL;
int id = -1;
char * host = NULL;
int port = -1;\
int sockfd;

/* Temperature Sensor Constants */
const int B = 4275;
const int R0 = 100000;

/* Read Temperature Sensor Value */
float read_temp() {
  int a = mraa_aio_read(temperature);
  float R = 1023.0/a-1.0;
  float tempval = 1.0/(log(R)/B+1/298.15)-273.15;
  if (SCALE == 'C')
    return tempval;
  else
    return tempval*9/5 + 32;
}

/* Get Local Time */
int get_localtime(char * buf, char on_exit) {
  time_t rawtime;
  struct tm *curr;
  time(&rawtime);
  curr = localtime(&rawtime);
  long long sec_elapsed = (curr->tm_hour - last_time.tm_hour)*3600 + (curr->tm_min - last_time.tm_min)*60 + (curr->tm_sec - last_time.tm_sec);
  if (sec_elapsed >= SAMPLE_PERIOD) {
    last_time.tm_hour = curr->tm_hour;
    last_time.tm_min = curr->tm_min;
    last_time.tm_sec = curr->tm_sec;
    return sprintf(buf, "%.2d:%.2d:%.2d", curr->tm_hour, curr->tm_min, curr->tm_sec);
  }
  if (on_exit)
    return sprintf(buf, "%.2d:%.2d:%.2d", curr->tm_hour, curr->tm_min, curr->tm_sec);
  return -1;  /* Not ready for sampling yet */
}

/* GPIO Interrupt on Button Press */
void handle_button_press() {
  int n;
  char buf[64];
  if (get_localtime(buf, 1) > 0) {
    n = sprintf(buf, "%s SHUTDOWN\n", buf);
    SSL_write(ssl, buf, n);
    if (logfd) {
      write(logfd, buf, n);
    }
  }
  if (command) {
    free(command);
  }
  mraa_aio_close(temperature);
  SSL_shutdown(ssl);
  SSL_free(ssl);
  exit(0);
}

/* Handle user input commands */
void handle_command(char * command) {
  /* Ignore leading whitespace */
  while (*command == ' ' || *command == '\t')
    command++;
  if (strstr(command, "LOG") == command) {
    return;
  }
  if (strstr(command, "SCALE=F") == command) {
    SCALE = 'F';
    return;
  }
  if (strstr(command, "SCALE=C") == command) {
    SCALE = 'C';
    return;
  }
  if (strstr(command, "PERIOD=") == command) {
    command += 7;
    int i = 0;
    while (command[i] != '\0') {
      if (!isdigit(command[i]))
        return;
      i++;
    }
    SAMPLE_PERIOD = atoi(command);
    return;
  }
  if (strstr(command, "STOP") == command) {
    RUN_FLAG = 0;
    return;
  }
  if (strstr(command, "START") == command) {
    RUN_FLAG = 1;
    return;
  }
  if (strstr(command, "OFF") == command) {
    handle_button_press();
    return;
  }
}

int main(int argc, char *argv[]) {
  /* Configure MRAA Inputs */
  temperature = mraa_aio_init(TEMPERATURE_MRAA);
  if (temperature == NULL) {
    fprintf(stderr, "Failed to initialize Analog Temperature Sensor Input\n");
    mraa_deinit();
    exit(1);
  }

  /* Parse possible --threads and --iterations options */
  struct option long_options[] = {
    {"period",      required_argument,  0,  'p'}, // Sampling interval in seconds
    {"scale",       required_argument,  0,  's'}, // Fahrenheit or Celsius
    {"log",         required_argument,  0,  'l'}, // Append reports to logfile
    {"id",          required_argument,  0,  'i'},
    {"host",        required_argument,  0,  'h'},
    {0, 0, 0, 0}
  };
  char * logfile = NULL;
  int opt_code;
  while ((opt_code = getopt_long(argc, argv, "", long_options, 0)) != -1) {
    switch (opt_code) {
      case 'p':
        if (optarg) {
          int i = 0;
          while (optarg[i] != '\0') {
            if (!isdigit(optarg[i])) {
              fprintf(stderr, "Invalid --period argument: %s\n", optarg);
              exit(1);
            }
            i++;
          }
          SAMPLE_PERIOD = atoi(optarg);
        }
        break;
      case 's':
        if (strlen(optarg) != 1) {
          fprintf(stderr, "Invalid --scale argument: %s\n", optarg);
          exit(1);
        }
        switch (optarg[0]) {
          case 'F':
          case 'C':
            SCALE = optarg[0];
            break;
          default:
            fprintf(stderr, "Invalid --scale argument: %s\n", optarg);
            exit(1);
        }
        break;
      case 'l':
        logfile = optarg;
        break;
      case 'i':
        if (optarg && strlen(optarg) == 9) {
          int i = 0;
          while (optarg[i] != '\0') {
            if (!isdigit(optarg[i])) {
              fprintf(stderr, "Invalid --id argument: %s\n", optarg);
              exit(1);
            }
            i++;
          }
          id = atoi(optarg);
        } else {
          fprintf(stderr, "Invalid --id argument: %s\n", optarg);
          exit(1);
        }
        break;
      case 'h':
        host = optarg;
        break;
      default:
        exit(1);
    }
  }

  /* Check Port Argument */
  if (optind != argc - 1) {
    fprintf(stderr, "Error, expected one port number argument but received %d\n", argc - optind);
    exit(1);
  }
  int i = 0;
  while (argv[optind][i] != '\0') {
    if (!isdigit(argv[optind][i])) {
      fprintf(stderr, "Invalid port argument: %s\n", argv[optind]);
      exit(1);
    }
    i++;
  }
  port = atoi(argv[optind]);

  /* Check that mandatory args are supplied */
  if (id == -1 || host == NULL || port == -1) {
    fprintf(stderr, "Error, please supply a valid id, host name/address, and port number\n");
    exit(1);
  }

  /* Create file if it doesnt exist and append */
  if (logfile) {
    logfd = creat(logfile, 0666);
    if (logfd < 0) {
      fprintf(stderr, "Error with logfile, could not creat %s: %s\n", logfile, strerror(errno));
      exit(2);
    }
  }

  /* Open a TCP Connection */
  struct sockaddr_in serv_addr;
  struct hostent *server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
     fprintf(stderr, "Error opening socket: %s", strerror(errno));
     exit(2);
  }
  server = gethostbyname(host);
  if (server == NULL) {
      fprintf(stderr,"Error, no such host\n");
      exit(2);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);
  serv_addr.sin_port = htons(port);
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    fprintf(stderr,"Error while connecting: %s\n", strerror(errno));
    exit(2);
  }

  /* SSL */
  SSL_library_init();
  SSL_load_error_strings();
  OpenSSL_add_all_algorithms();
  SSL_CTX * ctx = SSL_CTX_new(TLSv1_client_method());
  if (ctx == NULL) {
    fprintf(stderr, "Error, failed to get SSL context\n");
    exit(2);
  }
  if ((ssl = SSL_new(ctx)) == NULL) {
    fprintf(stderr, "Error, failed to complete SSL setup\n");
    exit(2);
  }
  if (!SSL_set_fd(ssl, sockfd)) {
    fprintf(stderr, "Error, failed to associate fd and SSL\n");
    exit(2);
  }
  if (SSL_connect(ssl) != 1) {
    fprintf(stderr, "Error, TLS/SSL handshake failed\n");
    exit(2);
  }

  int n;
  char id_buf[13];
  n = sprintf(id_buf, "ID=%d\n", id);
  SSL_write(ssl, id_buf, n);
  if (logfd) {
    write(logfd, id_buf, n);
  }

  /* Configure Timing */
  last_time.tm_hour = 0;
  last_time.tm_min = 0;
  last_time.tm_sec = 0;

  /* Poll for user input */
  struct pollfd fds[1];
  fds[0].fd = sockfd;
  fds[0].events = POLLIN;

  char buf[1024];
  command = malloc(sizeof(char));
  if (command == NULL) {
    fprintf(stderr, "Dynamic allocation of input buffer failed: %s\n", strerror(errno));
    exit(2);
  }

  int command_size = 0;
  while (1) {
    int ret = poll(fds, 1, 0);
    if (ret < 0) {
      fprintf(stderr, "Error while polling: %s\r\n", strerror(errno));
      exit(2);
    }
    /* Read from user input */
    if (ret > 0) {
      n = SSL_read(ssl, buf, 1024);
      if (n < 0) {
        fprintf(stderr, "Read from stdin failed: %s\r\n", strerror(errno));
        exit(2);
      }
      int i;
      for (i = 0; i < n; i++) {
        command = realloc(command, sizeof(char)*++command_size);
        if (buf[i] == '\n') {
          command[command_size-1] = '\n';
          if (logfd) {
            write(logfd, command, command_size);
          }
          command[command_size-1] = '\0';
          handle_command(command);
          command_size = 0;
        }
        else {
          command[command_size-1] = buf[i];
        }
      }

    }
    /* Read and output temperature and time */
    if (RUN_FLAG) {
      if ((n = get_localtime(buf, 0)) > 0) {
        float rval = read_temp();
        n = sprintf(buf, "%s %.1f\n", buf, rval);
        SSL_write(ssl, buf, n);
        if (logfd) {
          write(logfd, buf, n);
        }
      }
    }
  }
  return 0;
}
