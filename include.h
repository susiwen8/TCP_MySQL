#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/sem.h>
#include <pthread.h>
#include <semaphore.h>
#include <mysql.h>

#define BUF_SIZE 100
#define N_CLIENTS 2
#define CLEerr -100
#define SEMerr -103
#define WELCOME_MESSAGE "\nWelcome, what can I do for you\n1.Rebounding message\n2.Download file\n"
#define QUERY_NAME "select * from users where name="
#define QUERY_ADD "insert into users values(\""
#define VERIFY "Do you have a account?(Y/N)"
#define NO_ACCOUNT_MESSAGE "\nUser name doesn't exist!!!\nDo you want to create an account?(Y/N)\n"
#define YES "Y"
#define CREATE_ACCOUNT "You wanna create an account?(Y/N)"
#define INPUT_PASSWORD "Entre password:"
#define CONFIRM "Confirm your password:"
#define SUCCESS "Account create success!!!\n"
#define FAIL "Account create failed!!!\n"
#define ENTRE_NAME "Entre user name:"

sem_t sem;
pthread_mutex_t mutex;

void rebound(int clnt_sock);
void sendFile(int clnt_sock);
void sendMessage(struct sockaddr_in serv_addr, int sock);
void download(struct sockaddr_in serv_addr, int sock);
void file(int clnt_sock);
void *thread(void *p);
void semaphore();
char *mysql(char *id, int clnt_sock);
void code(int clnt_sock);
void add_user(int clnt_sock, char *name, char *new_password);
char *new_account_password(int clnt_sock);
