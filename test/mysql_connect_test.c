#include <stdio.h>
#include <my_global.h>
#include <mysql.h>

#include "../MySQL_Interface/mysql_api.h"
//typedef char[30] str;

int main( int argc, char* argv[])
{
	char *ip, *username, *password, *database;
	int port = 0;

	if( argc >= 4)
	{
		ip = argv[1];
		username = argv[2];
		password = argv[3];
		if( argc == 5) database = argv[4];
		if( argc == 6) port = atoi( argv[5]);
	}
	else{
		fprintf( stderr, "Correct Format: [ip] [username] [password]\n");
		exit(1);
	}

	sql_h db_h = init_h();

	connect_db( db_h, ip, username, password, database, port);

	//db_cmd( db_h, "CREATE DATABASE testdb");

	return 0;
}
