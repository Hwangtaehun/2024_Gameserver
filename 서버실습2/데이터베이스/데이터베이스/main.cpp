#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main()
{
	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;

	char* server = "localhost";
	char* user = "mysejong";
	char* password = "sj4321";
	char* database = "librarydb";


	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		exit(1);
	}
	if (mysql_query(conn, "show tables"))
	{
		exit(1);
	}

	res = mysql_use_result(conn);
	printf("MYSQL Tables in mysql database : \n");
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s \n", row[0]);


	if (mysql_query(conn, "SELECT * FROM member"))
	{
		return 1;
	}

	res = mysql_use_result(conn);

	printf("Returning List of Names : \n");
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s %s %s \n", row[0], row[1], row[2]);


	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}



