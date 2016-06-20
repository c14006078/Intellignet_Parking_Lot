#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>
#include<time.h>
int main(int argc,char* argv[]){
	MYSQL *conn_ptr;
	MYSQL mysql;
	MYSQL_ROW row;
	MYSQL_RES *result;
	char id[]="1";
	char state[]="1";
	char car_id[]="245-CBA";
	char p_query[99] = "UPDATE parkingstate SET state=";
	char s_query[99] = "UPDATE parkingfee SET outtime='";
	char n_query[99] = "INSERT INTO parkingfee (carid,intime,outtime,fee) VALUES ('";
	strcat(p_query,state);
	strcat(p_query," WHERE id=");
	strcat(p_query,id);
	p_query[strlen(p_query)]='\0';
	unsigned int num_fields;
	unsigned int i;
	conn_ptr = mysql_init(NULL);
	if(!conn_ptr)
	{
		printf("init_failed\n");
	}
	conn_ptr = mysql_real_connect(conn_ptr,"140.116.247.20","root","root","iparking",0,NULL,0);
	if(conn_ptr){
		// parking_state
		/*
		if(!mysql_query(conn_ptr,p_query))
		{
			result = mysql_store_result(conn_ptr);
			if(!result){
				printf("error");
				exit(1);
			}
		}
		while((row = mysql_fetch_row(result))!=NULL)
		printf("%s \n",row[1]);
		*/
		
		//car_state
		/*
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
				flag=1;
				if(strcmp(row[3],"000000000000")==0){
					strcat(s_query,realtime);
					strcat(s_query,"' WHERE carid='");
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

		*/

	}
	else{
		printf("no");
	}
	mysql_close(conn_ptr);
}
