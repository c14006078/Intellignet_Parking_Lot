#include <stdio.h>
#include <my_global.h>
#include <mysql.h>

#include "mysql_api.h"

sql_h init_h()
{
	sql_h con_h = (sql_h) malloc( sizeof( MYSQL));
	con_h = mysql_init( NULL);
	#ifdef SQL_DEBUG
		printf("connection_handler init\n");
	#endif
	return con_h;
}

void connect_db( sql_h con_h, char* ip, char* username, char* password, char* database, int port)
{
	if( mysql_real_connect( con_h, ip, username, password, database, port, NULL, 0) == NULL)
		error_call( con_h);
	#ifdef SQL_DEBUG
		printf("Already connect with DB.\nip			: %s\nusername: %s\npassword: %s\n", ip, username, password);
	#endif
}

void db_cmd( sql_h con_h, char* cmd)
{
	if( mysql_query( con_h, cmd))
		error_call( con_h);
	#ifdef SQL_DEBUG
		printf("cmd: %s", cmd);
	#endif
}

sql_r ret_store( sql_h con_h)
{
	sql_r result = (sql_r) malloc( sizeof( MYSQL_RES));
	result = mysql_store_result( con_h);

	if( result == NULL) error_call( con_h);
	#ifdef SQL_DEBUG
		printf("result not NULL\n");
	#endif

	return result;
}

Bool ret_get_row( sql_r result)
{
	row = mysql_fetch_row( result);
	return row ? true : false;
}

int num_ret_fields( sql_r result)
{
	return mysql_num_fields( result);
}

Bool ret_get_fields( sql_r result)
{
	field = mysql_fetch_field( result);
	return field? true : false;
}

void error_call( sql_h con_h)
{
	fprintf( stderr, "%s\n", mysql_error( con_h));
	mysql_close( con_h);
	exit(1);
}
