#include "include.h"

char *mysql(char *id) 
{
    strcat(id, "\"");
    char name[20] = {"\""};
    strcat(name, id);
    MYSQL      *conn;
    MYSQL_RES  *res;
    MYSQL_ROW  row;
    char *server = "localhost";
    char *user = "root";
    char *password = "132255SINAblog!"; /* set me first */
    char *database = "account";
    char query[100] = {0};
    strcpy(query, QUERY);
    int t;
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        printf("Error connecting to database %s: %s\n", database, mysql_error(conn));
    }
    else
    {
        printf("Connected....\n");
    }
    strcat(query, name);
    t = mysql_query(conn, query);
    if(0 != t)
    {
        printf("Error making query: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Query made...\n");
        res = mysql_store_result(conn);
        if(res)
        {
            row = mysql_fetch_row(res);
            if(row < 0) 
            {exit(EXIT_FAILURE);}
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return row[1]; 
}
