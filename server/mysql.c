#include "include.h"

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "132255SINAblog!"

const char success[] = SUCCESS;
const char fail[] = FAIL;
const char *server = SERVER;
const char *user = USER;
const char *password = PASSWORD;
MYSQL      *conn;
MYSQL_RES  *res;
MYSQL_ROW  row;
MYSQL_FIELD *field;

void mysql(char *database)
{
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        printf("Error connecting to database %s: %s\n", database, mysql_error(conn));
    }
    else
    {
        printf("Connected....\n");
    }
}


void get_download_list(int clnt_sock)
{
    char *database = "download";
    char query_list[100] = {0};
    char buffer[BUF_SIZE] = {0};
    strcpy(query_list, QUERY_LIST);
    mysql(database);
    int t = mysql_query(conn, query_list);
    printf("%s\n", query_list);
    if(0 != t)
    {
        printf("\nError making query: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Query made...\n");
        res = mysql_store_result(conn);
        if(res)
        {
            sprintf(buffer, "%llu", mysql_num_rows(res));//将行数传给客户端
            write(clnt_sock, buffer, sizeof(buffer));
            read(clnt_sock, buffer, sizeof(buffer));
            int i;
            while(row = mysql_fetch_row(res))
            {
                for(i = 0; i < mysql_num_fields(res); i++)
                {
                    if(0 == i)
                    {
                        while(field = mysql_fetch_field(res))
                        {
                            printf("\n%s\n", field->name);
                        }
                    }
                    strcpy(buffer, row[i]);
                    write(clnt_sock, buffer, sizeof(buffer));
                    printf("%s\n", row[i]);
                    read(clnt_sock, buffer, sizeof(buffer));
                }
            }
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
}

char *get_filePath(char *name)
{
    char *database = "download";
    char query_path[100] = {0};
    strcpy(query_path, QUERY_PATH);
    strcat(query_path, name);
    strcat(query_path, "\"");
    mysql(database);
    int t = mysql_query(conn, query_path);
    printf("%s\n", query_path);
    if(0 != t)
    {
        printf("\nError making query: %s\n", mysql_error(conn));
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
    return row[0]; 
}

char *get_password_from_mysql(char *id, int clnt_sock) 
{
    const char no_account_message[] = NO_ACCOUNT_MESSAGE;
    char yes_no[1] = {0};
    strcat(id, "\"");
    char name[20] = {"\""};
    strcat(name, id);
    char *database = "account";
    char query_name[100] = {0};
    strcpy(query_name, QUERY_NAME);
    strcat(query_name, name);//数据库命令
    mysql(database);//连接数据库
    int t = mysql_query(conn, query_name);
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
    return row[0]; 
}

void add_user(int clnt_sock, char *name, char *new_password)
{
    char *database = "account";
    char query_add[100] = {0};
    strcpy(query_add, QUERY_ADD);
    strcat(query_add, name);
    strcat(query_add, "\",\"");
    strcat(query_add, new_password);
    strcat(query_add, "\"");
    strcat(query_add, ")");
    mysql(database);//连接数据库
    int t = mysql_query(conn, query_add);//数据库命令
    printf("%s\n", query_add);
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
    mysql_close(conn);
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
