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

#define BAUDRATE B9600
#define MODEMDEVICE_fence "/dev/ttyACM0"
#define MODEMDEVICE_ultra "/dev/ttyACM1"
#define _POSIX_SOURCE 1

int main()
{
	int pipe_h2d[2];
	int pipe_d2h[2];
	pipe( pipe_h2d);
	pipe( pipe_d2h);
	/*pipe[0] for read
	  pipe[1] for write*/

	pid_t pid = fork();

	if( pid > 0)
	{
		//printf("Parent Ready!!! Wait for Child\n");

		close( pipe_h2d[0]);
		close( pipe_d2h[1]);
		
		/*arduino*/
		int fd_fence, res_fence;
		int fd_ultra, res_ultra;
		struct termios oldtio, newtio;
		struct termios oldtio2, newtio2;
		char buf_fence[256];
		char buf_ultra[256];

    		printf("Start... and press something to continue\n");
		getchar();
		// Set serial port for ultra and fence
   		fd_fence = open(MODEMDEVICE_fence, O_RDWR|O_NOCTTY);
		fd_ultra = open(MODEMDEVICE_ultra, O_RDWR|O_NOCTTY);
    		if (fd_fence < 0) {
      			perror(MODEMDEVICE_fence);
      			exit(1);
    		}
		if (fd_ultra < 0) {
      			perror(MODEMDEVICE_ultra);
      			exit(1);
    		}
	
    		printf("Open...\n");
    		tcgetattr(fd_fence, &oldtio);
    		tcgetattr(fd_ultra, &oldtio2);
    		bzero(&newtio, sizeof(newtio));
		bzero(&newtio2, sizeof(newtio2));
		// confiduration
    		newtio.c_cflag, newtio2.c_cflag= BAUDRATE|CS8|CLOCAL|CREAD;
    		newtio.c_iflag, newtio2.c_iflag = IGNPAR;
    		newtio.c_oflag, newtio2.c_oflag = 0;
    		newtio.c_lflag, newtio2.c_oflag = 0;
    		newtio.c_cc[VTIME], newtio2.c_cc[VTIME]=0;
    		newtio.c_cc[VMIN], newtio2.c_cc[VMIN]=10;

	
    		// clean queue
    		tcflush(fd_fence, TCIFLUSH);
		tcflush(fd_ultra, TCIFLUSH);
    		tcsetattr(fd_fence, TCSANOW, &newtio);
		tcsetattr(fd_ultra, TCSANOW, &newtio2);


    		printf("Reading...\n");
    		while(1)
		{
	  	// read for fence
      
      			res_fence = read(fd_fence, buf_fence, 1);

      			buf_fence[res_fence]= '\0';
	  		if(buf_fence[0] == 'c' ){
			  	/* notify main board to recognize 
					if success, print 'y' to fence arduino
			  	*/
				int i=0, end = 0;
				char buf[50];
				char c[1];
				c[1] = 'm';
				while(1)
				{
					//getchar();
					//printf("\ntimes %d\n", i);
			
					end = 0;
					char c[1];
					c[1] = 'm';


					write( pipe_h2d[1], c, sizeof(char));
					//printf("\nwrite to alpr success\n");

					while(1)
					{
						if( end = read( pipe_d2h[0], buf, 50))
						{
							printf("end=%d", end);
							if(end <6)
							{
								write( pipe_h2d[1], c, sizeof(char));
								printf("keyspace: Enter");
								sleep(0.5);
							}
							else
								break;
							/*if(end > 5){
								buf[end+1] = '\0';
								printf("end=%s", buf);
							}*/
							sleep(0.5);
							buf[end+1] = '\0';
							printf("\n\nparent get: %s\n\n", buf);
							//i++;
						}
			  		}
					if( end == 6)
					{
						write(fd_fence, "y\n", 2);		  
						break;
					}
				}
			}

			printf("res=%d buf=%s\n", res_fence, buf_fence);
		  
	 
		  	// read for ultrasonic
		  	res_ultra = read(fd_ultra, buf_ultra, 3);
	      		buf_ultra[res_ultra]= '\0';
	      		printf("res=%d buf=%s\n", res_ultra, buf_ultra);	  
		  	/*
		  	update data for parking lot  
		  	*/
	     		sleep(1) ;
		}
		


		/*\arduino*/


		printf("Close...\n");
    		close(fd_fence);
		close(fd_ultra);
    		tcsetattr(fd_fence, TCSANOW, &oldtio);
		tcsetattr(fd_ultra, TCSANOW, &oldtio2);
    		return 0;
	}
	else
	{
		printf("Child Process Ready!!!\n");
		close( pipe_h2d[1]);
		close( pipe_d2h[0]);

		dup2( pipe_h2d[0], STDIN_FILENO);
		dup2( pipe_d2h[1], STDERR_FILENO);

		close( pipe_h2d[0]);
		close( pipe_d2h[1]);

		execlp("alpr", "alpr", "-n 1","/dev/video0", NULL);

		exit(0);
	}
}
