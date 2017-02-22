#include "include.h"

const char success[] = SUCCESS;
const char fail[] = FAIL;

char *mysql(char *id, int clnt_sock) 
{
    const char no_account_message[] = NO_ACCOUNT_MESSAGE;
/*    const char input_password[] = INPUT_PASSWORD;
    const char confirm[] = CONFIRM;
    const char success[] = SUCCESS;
    const char fail[] = FAIL;*/
    char yes_no[1] = {0};
    //char new_password[20] = {0};
    //char confirm_password[20] = {0};
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
    char query_name[100] = {0};
    strcpy(query_name, QUERY_NAME);
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
    strcat(query_name, name);
    t = mysql_query(conn, query_name);
    printf("%s\n", query_name);
    if(0 != t)
    {
        printf("\nError making query: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Query made...\n");
        /***加入一个创建新帐户的函数*****/
        /*write(clnt_sock, no_account_message, sizeof(no_account_message));
        read(clnt_sock, yes_no, sizeof(yes_no));
        if(0 == strcmp(YES, yes_no))
        {
            char *new_password = new_account_password(clnt_sock);
            add_user(clnt_sock, name, new_password);
        }*/
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

void add_user(int clnt_sock, char *name, char *new_password)
{
    MYSQL      *conn;
    MYSQL_RES  *res;
    MYSQL_ROW  row;
    char *server = "localhost";
    char *user = "root";
    char *password = "132255SINAblog!"; /* set me first */
    char *database = "account";
    char query_add[100] = {0};
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        printf("Error connecting to database %s: %s\n", database, mysql_error(conn));
    }
    else
    {
        printf("Connected....\n");
    } 
    strcpy(query_add, QUERY_ADD);
    strcat(query_add, name);
    strcat(query_add, "\",\"");
    strcat(query_add, new_password);
    strcat(query_add, "\"");
    strcat(query_add, ")");
    int t = mysql_query(conn, query_add);
    if(0 == t)
    {
        printf("New user has successfully add into database!\n");
        write(clnt_sock, success, sizeof(success));
    }
    else
    {
        printf("\nError making query: %s\n", mysql_error(conn));
        write(clnt_sock, fail, sizeof(fail));
        exit(EXIT_FAILURE);
    }
}

char *new_account_password(int clnt_sock)
{
    const char input_password[] = INPUT_PASSWORD;
    const char confirm[] = CONFIRM;
    char *password = malloc(20 * sizeof(char));
    char new_password[20] = {0};
    char confirm_password[20] = {0};
    write(clnt_sock, input_password, sizeof(input_password));    
    read(clnt_sock, new_password, sizeof(new_password));
    printf("new_password:%s\n", new_password);
    write(clnt_sock, confirm, sizeof(confirm));
    read(clnt_sock, confirm_password, sizeof(confirm_password));
    printf("confirm_password:%s\n", confirm_password);
    if(0 == strcmp(confirm_password, new_password))
    {
        write(clnt_sock, success, sizeof(success));
    }
    else
    {
        write(clnt_sock, fail, sizeof(fail));
        exit(EXIT_FAILURE);
    }
    strcpy(password, new_password);
    return password;
}
