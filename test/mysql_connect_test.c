#include <stdio.h>
#include <my_global.h>
#include <mysql.h>

#include "../MySQL_Interface/mysql_api.h"
//typedef char[30] str;

int main( int argc, char* argv[])
{
	char *ip, *username, *password;
	if( argc == 4)
	{
		ip = argv[1];
		username = argv[2];
		password = argv[3];
	}
	else{
		fprintf( stderr, "Correct Format: [ip] [username] [password]\n");
		exit(1);
	}

	sql_h db_h = init_h();

	connect_db( db_h, ip, username, password);

	db_cmd( db_h, "CREATE DATABASE testdb");

	return 0;
}
