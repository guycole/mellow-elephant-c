/*
** Title:serial.h
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
**   $Id$
**
**   $Log$
*/

#ifndef __SERIAL_H__
#define __SERIAL_H__

int do_command(int fd, char *command, char *answer);
int setup_port(int fd);
int open_port(char *port);

#endif /* __SERIAL_H__ */
