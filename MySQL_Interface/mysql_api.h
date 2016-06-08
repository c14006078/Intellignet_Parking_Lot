typedef MYSQL *sql_h;

sql_h init_h( void);

void connect_db( sql_h con_h, char* ip, char* username, char* password);

void db_cmd( sql_h con_h, char* cmd);

void error_call( sql_h con_h);
