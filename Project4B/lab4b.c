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
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <mraa.h>
#include <math.h>
#include <time.h>
#include <poll.h>
#include <fcntl.h>
#include <ctype.h>

#define TEMPERATURE_MRAA 1
#define BUTTON_MRAA 60

mraa_gpio_context button;
mraa_aio_context temperature;
struct tm last_time;

/* Default Settings */
int SAMPLE_PERIOD = 1;
char SCALE = 'F';
char RUN_FLAG = 1;
int logfd = 0;
char * command = NULL;

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
  if ((n = get_localtime(buf, 1)) > 0) {
    fprintf(stdout, "%s SHUTDOWN\n", buf);
    if (logfd) {
      write(logfd, buf, n);
      write(logfd, " SHUTDOWN\n", 10);
    }
  }
  if (command) {
    free(command);
  }
  mraa_gpio_close(button);
  mraa_aio_close(temperature);
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
  button = mraa_gpio_init(BUTTON_MRAA);
  if (button== NULL) {
    fprintf(stderr, "Failed to initialize GPIO Button Input\n");
    mraa_deinit();
    exit(1);
  }
  temperature = mraa_aio_init(TEMPERATURE_MRAA);
  if (temperature == NULL) {
    fprintf(stderr, "Failed to initialize Analog Temperature Sensor Input\n");
    mraa_deinit();
    exit(1);
  }
  mraa_gpio_dir(button, MRAA_GPIO_IN);
  mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &handle_button_press, NULL);

  /* Parse possible --threads and --iterations options */
  struct option long_options[] = {
    {"period",      required_argument,  0,  'p'}, // Sampling interval in seconds
    {"scale",       required_argument,  0,  's'}, // Fahrenheit or Celsius
    {"log",         required_argument,  0,  'l'}, // Append reports to logfile
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
              fprintf(stderr, "Usage: ./lab4b [--period=seconds] [--scale=F/C] [--log=filename]\n");
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
          fprintf(stderr, "Usage: ./lab4b [--period=seconds] [--scale=F/C] [--log=filename]\n");
          exit(1);
        }
        switch (optarg[0]) {
          case 'F':
          case 'C':
            SCALE = optarg[0];
            break;
          default:
            fprintf(stderr, "Invalid --scale argument: %s\n", optarg);
            fprintf(stderr, "Usage: ./lab4b [--period=seconds] [--scale=F/C] [--log=filename]\n");
            exit(1);
        }
        break;
      case 'l':
        logfile = optarg;
        break;
      default:
        fprintf(stderr, "Usage: ./lab4b [--period=seconds] [--scale=F/C] [--log=filename]\n");
        exit(1);
    }
  }

  /* Create file if it doesnt exist and append */
  if (logfile) {
    logfd = creat(logfile, 0666);
    if (logfd < 0) {
      fprintf(stderr, "Error with logfile, could not creat %s: %s\n", logfile, strerror(errno));
      exit(1);
    }
  }

  /* Configure Timing */
  last_time.tm_hour = 0;
  last_time.tm_min = 0;
  last_time.tm_sec = 0;

  /* Poll for user input */
  struct pollfd fds[1];
  fds[0].fd = 0;
  fds[0].events = POLLIN;

  char buf[1024];
  command = malloc(sizeof(char));
  if (command == NULL) {
    fprintf(stderr, "Dynamic allocation of input buffer failed: %s\n", strerror(errno));
    exit(1);
  }

  int command_size = 0;
  int n;
  while (1) {
    int ret = poll(fds, 1, 0);
    if (ret < 0) {
      fprintf(stderr, "Error while polling: %s\r\n", strerror(errno));
      exit(1);
    }
    /* Read from user input */
    if (ret > 0) {
      n = read(fds[0].fd, &buf, 1024);
      if (n < 0) {
        fprintf(stderr, "Read from stdin failed: %s\r\n", strerror(errno));
        exit(1);
      }
      int i;
      for (i = 0; i < n; i++) {
        command = realloc(command, sizeof(char)*++command_size);
        if (buf[i] == '\n') {
          command[command_size-1] = '\0';
          if (logfd) {
            write(logfd, command, command_size);
            write(logfd, "\n", 1);
          }
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
        fprintf(stdout, "%s ", buf);
        if (logfd) {
          write(logfd, buf, n);
          write(logfd, " ", 1);
        }
        float rval = read_temp();
        fprintf(stdout, "%.1f\n", rval);
        n = sprintf(buf,  "%.1f\n", rval);
        if (logfd) {
          write(logfd, buf, n);
        }
      } 
    }
  }
  return 0;
}