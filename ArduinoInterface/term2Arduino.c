#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "term2Arduino.h"

//#define fdn int


int shake_hand( char* bridge, speed_t baudrate, p_termios t_option)
{
  int fd;
  fd = open( bridge, O_RDWR | O_NOCTTY);//TODO:Argument need to survey 
  
  if( fd < 0){ perror("shake hand fault\n"); return -1;}
  else{ printf(" Shake Hand Success!\n");}

  //usleep( 350000);

  //get current serial port setting
  tcgetattr( fd, t_option);
  
  //set baudrate
  cfsetispeed( t_option, baudrate);
  cfsetospeed( t_option, baudrate);

  //8-bit, no parity, no stop bit
  //TODO:survey how to get more 
  t_option->c_cflag &= ~PARENB;
  t_option->c_cflag &= ~CSTOPB;
  t_option->c_cflag &= ~CSIZE;
  t_option->c_cflag |= CS8;
  t_option->c_cflag |= ICANON;//Canonical mode

  //commit the serial port setting
  tcsetattr( fd, TCSANOW, t_option);

  return fd;
}

int host_read( int fd, char* rbuf, int len)
{
  int n = read( fd, rbuf, len);
  rbuf[n] = 0;
  return n;
}

void host_write( int fd, char* wbuf, int len)
{
  write( fd, wbuf, len);
}
