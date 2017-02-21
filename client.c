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

    char user_name[20] = {0};
    char user_password[20] = {0};
    char id_client[BUF_SIZE] = {0};
    char buffer[BUF_SIZE] = {0};
    sprintf(id_client, "%ld", id);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    write(sock, id_client, sizeof(id_client));//将线程ID传给server
    printf("\nThis is client %ld\n", id);
    read(sock, buffer, sizeof(buffer));//服务器询问用户名
    printf("%s\n",buffer);
    memset(buffer, 0, sizeof(buffer));//清空buffer
    scanf("%s", user_name);//输入用户名
    write(sock, user_name, sizeof(user_name));//将用户名传给服务器
    read(sock, buffer, sizeof(buffer));//服务器询问密码
    printf("%s\n", buffer);
    memset(buffer, 0, sizeof(buffer));//清空buffer
    
    int i = 0;
    int count = 3;
    while(i < 3)
    {
        scanf("%s", user_password);//输入密码
        write(sock, user_password, sizeof(user_password));
        read(sock, buffer, sizeof(buffer));
        printf("%s", buffer);
        if(strcmp(buffer, WELCOME_MESSAGE) == 0)
            break;
        count--;
        printf("\nYou have %d to entre your password!\n", count);
        i++;
    }

    memset(buffer, 0, sizeof(buffer));
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

