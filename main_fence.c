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
#define MODEMDEVICE_fence "/dev/ttyACM0"
#define _POSIX_SOURCE 1

void parking_state(char*);
void car_state(char*);
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
		int fd_fence, res_fence = -1;
		struct termios oldtio, newtio;
		char buf_fence[256];

    	printf("Start... and press something to continue\n");
		getchar();
		// Set serial port for ultra and fence
   		fd_fence = open(MODEMDEVICE_fence, O_RDWR|O_NOCTTY);
    		if (fd_fence < 0) {
      			perror(MODEMDEVICE_fence);
      			exit(1);
    		}
    	printf("Open...\n");
    	tcgetattr(fd_fence, &oldtio);
    	bzero(&newtio, sizeof(newtio));
		// confiduration
    	newtio.c_cflag = BAUDRATE|CS8|CLOCAL|CREAD ;
		newtio.c_iflag = IGNPAR;
    	newtio.c_oflag = 0 ; 
    	newtio.c_lflag = 0 ;
    	newtio.c_cc[VTIME] = 0 ;
    	newtio.c_cc[VMIN] = 10 ;

	
    	// clean queue
    	tcflush(fd_fence, TCIFLUSH);
    	tcsetattr(fd_fence, TCSANOW, &newtio);


    	printf("Reading...\n");
    	while(1)
		{
	  	// read for fence
			
			res_fence = read(fd_fence, buf_fence, 255);
			int T = 0, k;
			for( k = 0; k < res_fence; k++)
			{
				if( buf_fence[k] == 'c')
				{
					T = 1;
				}
				buf_fence[k]='K';
			}

	  		if( T > 0 ){
			  	/* notify main board to recognize 
					if success, print 'y' to fence arduino
			  	*/
				int i=0, end = 0;
				char buf[50];
				char c[1];
				c[0] = 'm';
				while(1)
				{
					//getchar();
					//printf("\ntimes %d\n", i);
			
					end = 0;
					//printf("\nwrite to alpr success\n");
					write( pipe_h2d[1], c, sizeof(char));

					while(1)
					{
						if( (end = read( pipe_d2h[0], buf, 50)) > 0 )
						{
							printf("end=%d", end);
							buf[end+1] = '\0';
							printf("\n\nparent get: %s\n\n", buf);
							if(end != 6)
							{
								write( pipe_h2d[1], c, sizeof(char));
								printf("keyspace: Enter");
							}
							else
								break;
							/*if(end > 5){
								buf[end+1] = '\0';
								printf("end=%s", buf);
							}*/
							//i++;
						}
			  		}
					if( end == 6)
					{
						write(fd_fence, "y\n", 2);
						buf[6] = '\0';
						car_state(buf);		  
						break;
					}
				}
				printf("\n\nF**K\n\n");
			}

			printf("res=%d buf=%c\n", res_fence, buf_fence[0]);
		  
		}
		


		/*\arduino*/


		printf("Close...\n");
    	close(fd_fence);
    	tcsetattr(fd_fence, TCSANOW, &oldtio);
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

void car_state(char* car_id){
	MYSQL *conn_ptr;
	MYSQL mysql;
	MYSQL_ROW row;
	MYSQL_RES *result;
	char s_query[99] = "UPDATE parkingfee SET outtime='";
	char n_query[99] = "INSERT INTO parkingfee (carid,intime,outtime,fee) VALUES ('";
	conn_ptr = mysql_init(NULL);
	if(!conn_ptr)
	{
		printf("init_failed\n");
	}
	conn_ptr = mysql_real_connect(conn_ptr,"140.116.247.20","root","root","iparking",0,NULL,0);
	if(conn_ptr){
				
		//car_state
			
		time_t timep;
		struct tm *p;
	//	memset(p,0,sizeof(p));
		time(&timep);
		p=localtime(&timep);
		char realtime[99] ="";
		char year[5];
		char mon[5];
		char day[5];
		char hour[5];
		char min[5];
		sprintf(year,"%d",1900+p->tm_year);
		year[4]='\0';
		sprintf(mon,"%d",1+p->tm_mon);
		if(strlen(mon)==1){mon[1]=mon[0];mon[0]='0'; mon[2]='\0';}
		sprintf(day,"%d",p->tm_mday);
		if(strlen(day)==1){day[1]=day[0];day[0]='0'; day[2]='\0';}
		sprintf(hour,"%d",p->tm_hour);
		if(strlen(hour)==1){hour[1]=hour[0];hour[0]='0'; hour[2]='\0';}
		sprintf(min,"%d",p->tm_min);
		if(strlen(min)==1){min[1]=min[0];min[0]='0'; min[2]='\0';}
		strcat(realtime,year);
		strcat(realtime,mon);
		strcat(realtime,day);
		strcat(realtime,hour);
		strcat(realtime,min);
		realtime[strlen(realtime)]='\0';

		if(!mysql_query(conn_ptr,"SELECT * FROM parkingfee"))
		{
			result = mysql_store_result(conn_ptr);
			if(!result){
				printf("error");
				exit(1);
			}
		}
		int flag=0;
		while((row = mysql_fetch_row(result))!=NULL){

			// out
			if(strcmp(row[1],car_id)==0){
				if(strcmp(row[3],"000000000000")==0){
					flag=1;
					int fee;
					char in_year[5];
					in_year[0]=row[2][0];
					in_year[1]=row[2][1];
					in_year[2]=row[2][2];
					in_year[3]=row[2][3];
					in_year[4]='\0';
					char in_mon[3];
					in_mon[0]=row[2][4];
					in_mon[1]=row[2][5];
					in_mon[2]='\0';
					char in_day[3];
					in_day[0]=row[2][6];
					in_day[1]=row[2][7];
					in_day[2]='\0';
					char in_hour[3];
					in_hour[0]=row[2][8];
					in_hour[1]=row[2][9];
					in_hour[2]='\0';
					char in_min[3];
					in_min[0]=row[2][10];
					in_min[1]=row[2][11];
					in_min[2]='\0';
					int min = (in_min[0]-'0')*10+(in_min[1]-'0');
				
					printf("Y:%d M:%d D:%d H:%d MIN:%d \n",atoi(in_year),atoi(in_mon),atoi(in_day),atoi(in_hour),atoi(in_min));
					double count =0;
					struct tm p2;
					memset(&p2,0,sizeof(p2));
					p2.tm_year = atoi(in_year)-1900;
					p2.tm_mon = atoi(in_mon)-1;
					p2.tm_mday = atoi(in_day);
					p2.tm_hour = atoi(in_hour);
					p2.tm_min = atoi(in_min);
					time_t t1 = mktime(&p2);
					time_t t2 = mktime(p);
					//printf("%u   %u\n",(unsigned int)t1,(unsigned int)t2);
					float  time_diff = difftime(t2,t1);
					printf("diff: %f\n",time_diff);
					float t_hour= (float)(((time_diff/60/60)));
					int f_hour;
					f_hour = ceil(t_hour);
					printf("%d\n",f_hour);
					fee = f_hour*30;
					char f_fee[10]="";
					sprintf(f_fee,"%d",fee);
					f_fee[strlen(f_fee)]='\0';
					strcat(s_query,realtime);
					strcat(s_query,"',fee=");
					strcat(s_query,f_fee);
					strcat(s_query," WHERE carid='");
					strcat(s_query,row[1]);
					strcat(s_query,"' AND outtime='000000000000'");
					s_query[strlen(s_query)]='\0';
					printf("%s",s_query);
					if(!mysql_query(conn_ptr, s_query)){
					}
					else { printf("wrong1");}
				}
			
			}
		}
			// in
			if(flag==0){
				strcat(n_query,car_id);
				strcat(n_query,"','");
				strcat(n_query,realtime);
				strcat(n_query,"','000000000000',0)");
				printf("%s\n",n_query);
					if(!mysql_query(conn_ptr, n_query)){
					}
					else { printf("wrong2");}

			}
	}
	else (printf("no"));
	mysql_close(conn_ptr);

}
