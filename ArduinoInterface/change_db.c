#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>
#include<time.h>
#include<math.h>
void parking_state(char*);
void car_state(char*);
int main(int argc,char* argv[]){
	char test[]="1 0";
	parking_state(test);
	char car_id[]="321-CBA";
	car_state(car_id);

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
			result = mysql_store_result(conn_ptr);
			if(!result){
		
			}
		}
		
	}
	mysql_close(conn_ptr);
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
					char in_year[4];
					in_year[0]=row[2][0];
					in_year[1]=row[2][1];
					in_year[2]=row[2][2];
					in_year[3]=row[2][3];
					char in_mon[2];
					in_mon[0]=row[2][4];
					in_mon[1]=row[2][5];
					char in_day[2];
					in_day[0]=row[2][6];
					in_day[1]=row[2][7];
					char in_hour[2];
					in_hour[0]=row[2][8];
					in_hour[1]=row[2][9];
					char in_min[2];
					in_min[0]=row[2][10];
					in_min[1]=row[2][11];
					int min = (in_min[0]-'0')*10+(in_min[1]-'0');
				
					printf("%d %d %d %d %d \n",atoi(in_year),atoi(in_mon),atoi(in_day),atoi(in_hour),atoi(in_min));
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
					printf("%f\n",time_diff);
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
