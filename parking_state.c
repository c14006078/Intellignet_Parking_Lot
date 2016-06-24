#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <math.h>
#include <time.h>

#define BAUDRATE B9600
#define MODEMDEVICE_ultra "/dev/ttyACM1"
#define _POSIX_SOURCE 1

void parking_state(char*);
void car_state(char*);
int main()
{
		/*arduino*/
		int fd_ultra, res_ultra;
		struct termios oldtio2, newtio2;
		char buf_ultra[256];
		// Set serial port for ultra and fence
		fd_ultra = open(MODEMDEVICE_ultra, O_RDWR|O_NOCTTY);
		if (fd_ultra < 0) {
      			perror(MODEMDEVICE_ultra);
      			exit(1);
    	}
	
    	printf("Open...\n");
    	tcgetattr(fd_ultra, &oldtio2);
		bzero(&newtio2, sizeof(newtio2));
		// confiduration
		newtio2.c_cflag= BAUDRATE|CS8|CLOCAL|CREAD;
    	newtio2.c_iflag = IGNPAR;
		newtio2.c_oflag = 0;
		newtio2.c_oflag = 0;
		newtio2.c_cc[VTIME]=0;
		newtio2.c_cc[VMIN]=10;

	
    	// clean queue
		tcflush(fd_ultra, TCIFLUSH);
		tcsetattr(fd_ultra, TCSANOW, &newtio2);
    	printf("Reading...\n");
    	while(1)
		{
		  	// read for ultrasonic
		  	res_ultra = read(fd_ultra, buf_ultra, 3);
	      	buf_ultra[res_ultra]= '\0';
	      	printf("res=%d buf=%s\n", res_ultra, buf_ultra);
			parking_state(buf_ultra);	  
		  	/*
		  	update data for parking lot  
		  	*/
	     		sleep(1) ;
		}
		


		/*\arduino*/

		printf("Close...\n");
		close(fd_ultra);
		tcsetattr(fd_ultra, TCSANOW, &oldtio2);
    	return 0;
}
void parking_state(char* state){

	char p_query[99] = "UPDATE parkingstate SET state=";
	char buf[3];
	strcpy(buf,state);
	char s[2]="";
	char n[2]="";
	s[0] = buf[2];
	n[0] = buf[0];
	strcat(p_query,s);
	strcat(p_query," WHERE id=");
	strcat(p_query,n);
	p_query[strlen(p_query)]='\0';
	MYSQL *conn_ptr;
	MYSQL mysql;
	MYSQL_ROW row;
	MYSQL_RES *result;
	conn_ptr = mysql_init(NULL);
	if(!conn_ptr)
	{
		printf("init_failed\n");
	}
	conn_ptr = mysql_real_connect(conn_ptr,"140.116.247.20","root","root","iparking",0,NULL,0);
	if(conn_ptr){
		// parking_state
		
		if(!mysql_query(conn_ptr,p_query))
		{
			printf("change\n");
			result = mysql_store_result(conn_ptr);
			if(!result){
		
			}

		}
		
	}
	mysql_close(conn_ptr);
}
