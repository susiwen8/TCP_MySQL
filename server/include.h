#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mysql.h>

#define BUF_SIZE 100
#define N_CLIENTS 2
#define WELCOME_MESSAGE "\nWelcome, what can I do for you\n1.Rebounding message\n2.Download file\n"
#define QUERY_NAME "select password from users where name="
#define QUERY_ADD "insert into users values(\""
#define QUERY_LIST "select name from list"
#define QUERY_PATH "select path from list where name=\""
#define VERIFY "Do you have a account?(Y/N)"
#define NO_ACCOUNT_MESSAGE "\nUser name doesn't exist!!!\nDo you want to create an account?(Y/N)\n"
#define YES "Y"
#define CREATE_ACCOUNT "You wanna create an account?(Y/N)"
#define INPUT_PASSWORD "Entre password:"
#define CONFIRM "Confirm your password:"
#define SUCCESS "Account create success!!!\n"
#define FAIL "Account create failed!!!\n"
#define ENTRE_NAME "Entre user name:"


void rebound(int clnt_sock);
void sendFile(int clnt_sock);
void file(int clnt_sock);
char *get_password_from_mysql(char *id, int clnt_sock);
void code(int clnt_sock);
void add_user(int clnt_sock, char *name, char *new_password);
char *new_account_password(int clnt_sock);
void mysql(char *database);
void get_download_list(int clnt_sock);
char *get_filePath(char *name);
