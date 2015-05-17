/*
** Title:serial.c
**
** Description:
**
** Development Environment:
**   Fedora 8
**   gcc version 4.1.2 20070925 (Red Hat 4.1.2-33)
**
** Legalese:  
**   Copyright (C) 2008 Digital Burro, INC.
**
** Author:
**   G.S. Cole (guycole at gmail dot com)
**
** Maintenance History:
**   $Log$
*/
static char rcsid[] = "$Id$";

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "serial.h"

#define MAX_LINE 1024

/*
** Read data from the serial port.
** Blocking read.  Returns byte quantity.
*/
int read_port(int fd, char *string) {
  ssize_t counter;

  int finished;

  char *ndx;
  char data[MAX_LINE];

  bzero(data, MAX_LINE);
  bzero(string, MAX_LINE);

  finished = 0;  
  while (!finished) {
    counter = read(fd, data, MAX_LINE);
    strncat(string, data, counter);

    if (strncmp(&data[counter-1], "\n", 1) == 0 || strncmp(&data[counter-1], "\r", 1) == 0) { 
      finished = 1; 
    }
  }

  if (counter < 0) {
      perror("read serial: returned error code");
      exit(EXIT_FAILURE);
  }

  ndx = strchr(string, '\n');
  if (ndx != NULL) {
    *ndx = '\0';
  }

  ndx = strchr(string, '\r');
  if (ndx != NULL) {
    *ndx = '\0';
  }

  return(counter);
}

/*
** Write data to the serial port.
** Returns quantity of bytes written
*/
int write_port(int fd, char *string) {
  ssize_t counter, limit;

  limit = strlen(string);
  counter = write(fd, string, limit);
  
  if (counter < 0) {
    perror("write_serial: returned error code");
    exit(EXIT_FAILURE);
  }

  return(counter);
}

/*
** Write a command to the scanner and return the answer
** Returns read count or -1 if failure
*/
int do_command(int fd, char *command, char *answer) {
  char buffer[128];

  int counter;

  struct timespec remain_time;
  struct timespec sleep_time;
  
  /* pause a little, so we don't overwhelm the scanner */
  sleep_time.tv_nsec = 100000;
  sleep_time.tv_sec = 0;
  nanosleep(&sleep_time, &remain_time);

  /* write command to scanner */  
  snprintf(buffer, 128, "%s\r", command);
  counter = write_port(fd, buffer);

  if (counter != (strlen(command) +1)) {
    fprintf(stderr, "short write:%d", counter);
    return(-1);
  }

  counter = read_port(fd, answer);
  
  //  check_err(command, answer);  /* process ERR and NG replies */
  
  return(counter);
}

/*
** Port initializtion to 9600 8N1
*/
int setup_port(int fd) {
  struct termios port;

  tcgetattr(fd, &port);

  cfsetispeed(&port, B9600);
  cfsetospeed(&port, B9600);

  port.c_cflag &= ~CSIZE; /* Mask the character size bits */
  port.c_cflag |= CS8;    /* Select 8 data bits */

  /* set port to 8N1 */
  port.c_cflag &= ~PARENB;
  port.c_cflag &= ~CSTOPB;
  port.c_cflag &= ~CSIZE;
  port.c_cflag |= CS8;
  
  port.c_cflag |= (CLOCAL | CREAD);
  port.c_cflag &= ~CRTSCTS; /* this should turn off hdwr flow ctl */
  
  port.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  
  tcsetattr(fd, TCSAFLUSH, &port);
  
  return(1);
}

/*
** Open specified serial port.
** Returns file descriptor if success or -1 failure
*/
int open_port(char *port) {
  int fd;
  char error_string[128];

  fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    snprintf(error_string, 128, "open_port: unable to open %s", port);
    perror(error_string);
    exit(EXIT_FAILURE);
  } else {
    fcntl(fd, F_SETFL, 0); /* set blocking port */
  }

  return(fd);
}
