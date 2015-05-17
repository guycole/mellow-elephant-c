/*
** Title:main.c
**
** Description:
**   Command a Uniden Bearcat Scanner and get the results.
**
**   compile as: gcc -o sampler1 -Wall main.c serial.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "serial.h"

/*
**
*/
int main(int argc, char *argv[]) {
  int counter;
  int port_fd;

  char answer[132];
  char serial_port[32];

  //  printf("begin:%d\n", argc);

  if (argc != 2) {
    printf("usage: sampler command\n");
    return(EXIT_SUCCESS);
  }

  strncpy(serial_port, "/dev/ttyUSB0", 32);
  // strncpy(serial_port, "/dev/ttyS0", 32);
  // strncpy(serial_port, "/dev/ttyS1", 32);

  port_fd = open_port(serial_port);
  setup_port(port_fd);

  counter = do_command(port_fd, argv[1], answer);

 ///   printf("%d:%s:\n", counter, answer);
  printf("%s\n", answer);

  //  printf("end\n");

  exit(EXIT_SUCCESS);
}
