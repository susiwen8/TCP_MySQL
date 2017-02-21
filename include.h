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
#define WELCOME_MESSAGE "Welcome, what can I do for you\n1.Rebounding message\n2.DOwnload file\n"

sem_t sem;
pthread_mutex_t mutex;

void rebound(int clnt_sock);
void sendFile(int clnt_sock);
void sendMessage(struct sockaddr_in serv_addr, int sock);
void download(struct sockaddr_in serv_addr, int sock);
void file(int clnt_sock);
void *thread(void *p);
void semaphore();

typedef user_info
{
    char name[20];
    char password[20];
}USER_INFO;
