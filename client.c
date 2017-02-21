#include "include.h"

int main()
{
    while(1)
    {
        semaphore();
    }
    return 0;
}

void *thread(void* p)
{
    pthread_t id = pthread_self();
    sem_wait(&sem);
    pthread_mutex_lock(&mutex);
    
	int operation ;
    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口

    char id_client[BUF_SIZE] = {0};
    sprintf(id_client, "%ld", id);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    write(sock, id_client, sizeof(id_client));//将线程ID传给server
    printf("\nThis is client %ld\n", id);
    char buffer[BUF_SIZE] = {0};
    read(sock, buffer, sizeof(buffer));//获取从server回传的操作信息
    printf("%s", buffer);
    printf("Your choice:");
    scanf("%d", &operation);
    switch(operation)
    {
        case 1:
            {
                sendMessage(serv_addr, sock);
                break;
            }

        case 2:
            {
                download(serv_addr, sock);
                break;
            }

        default:
            break;
    }
   
    sem_post(&sem);
    pthread_mutex_unlock(&mutex);
}

