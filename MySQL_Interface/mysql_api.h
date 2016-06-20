typedef MYSQL *sql_h;
typedef MYSQL_RES *sql_r;
typedef enum Bool {false, true} Bool;

MYSQL_ROW row;
MYSQL_FIELD *field;

sql_h init_h( void);

void connect_db( sql_h con_h, char* ip, char* username, char* password, char* database, int port);

void db_cmd( sql_h con_h, char* cmd);

void error_call( sql_h con_h);
