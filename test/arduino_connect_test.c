#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "../ArduinoInterface/term2Arduino.h"

int main( int argc, char* argv[])
{
  int fd;
  char bridge[20];
  if( argc == 1) strcpy( bridge, defaultBridge);
  else strcpy( bridge, argv[1]);


  speed_t baudrate = B9600;
  p_termios option = (p_termios) malloc( sizeof( struct termios));
  fd = shake_hand( bridge, baudrate, option);

  char* ws = "Hello Arduino";
  printf("I say Hello to u\n");
  host_write( fd, ws, strlen( ws) + 1);

  char rbuf[20];
  host_read( fd, rbuf, 20);

  printf("host_read: %s\n", rbuf);
}
