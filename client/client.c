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
    
	int operation, i;
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
    char yes_no[1] = {0};
    sprintf(id_client, "%ld", id);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    write(sock, id_client, sizeof(id_client));//将线程ID传给server
    printf("\nThis is client %ld\n", id);
    read(sock, buffer, sizeof(buffer));//服务器询问是否有账户
    printf("%s", buffer);
    scanf("%s", yes_no);
    write(sock, yes_no, sizeof(yes_no));//告知服务器自己是否有账户
    if(0 == strcmp(yes_no, YES))//有帐号的情况
    {
        read(sock, buffer, sizeof(buffer));//服务器询问用户名
        printf("%s",buffer);
//        memset(buffer, 0, sizeof(buffer));//清空buffer
        scanf("%s", user_name);//输入用户名
        write(sock, user_name, sizeof(user_name));//将用户名传给服务器
        read(sock, buffer, sizeof(buffer));//服务器询问密码或者询问创建账户
        if(0 == strcmp(buffer, NO_ACCOUNT_MESSAGE))//服务器不存在该用户名
        {
            scanf("%s", yes_no);
            write(sock, yes_no, sizeof(yes_no));
            if(0 != strcmp(YES, yes_no))
            {
                exit(EXIT_FAILURE);
            }
            else
            {
                for(i = 0; i < 2; i++)
                {
                    read(sock, buffer, sizeof(buffer));
                    printf("%s", buffer);
                    scanf("%s", user_password);
                    write(sock, user_password, sizeof(user_password));
                }
                read(sock, buffer, sizeof(buffer));
                printf("%s", buffer);
                if(0 == strcmp(buffer, SUCCESS))
                {   
                 /*************/
                }
                else
                {
                    exit(EXIT_FAILURE);
                }
            }
        }
        printf("%s", buffer);//询问密码
        memset(buffer, 0, sizeof(buffer));//清空buffer
    
        i = 0;
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
            if(0 == count)
            {
                /****改进，不直接退出程序，而是进行下一个线程操作***/
                exit(EXIT_FAILURE);
            //    pthread_exit(0);
            //    pthread_cancel(id);
            }
            printf("\nYou have %d to entre your password!\nPassword:", count);
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
    else//没有账号
    {
        read(sock, buffer, sizeof(buffer));//服务器询问建立账户        
        printf("%s", buffer);
        scanf("%s", yes_no);//输入Y或者N
        write(sock, yes_no, sizeof(yes_no));//将选择传给服务器
        if(0 == strcmp(yes_no, YES))//选择Y
        {
            read(sock, buffer, sizeof(buffer));//服务器询问用户名
            printf("%s", buffer);
            scanf("%s", user_name);
            write(sock, user_name, sizeof(user_name));//将用户名传给服务器
            for(i = 0; i < 2; i++)//两次输入密码确认
            {
                read(sock, buffer, sizeof(buffer));
                printf("%s", buffer);
                scanf("%s", user_password);
                write(sock, user_password, sizeof(user_password));
                memset(buffer, 0, sizeof(buffer));
            }
            read(sock, buffer, sizeof(buffer));//服务器回传密码设置信息
            printf("%s", buffer);
            if(0 == strcmp(FAIL, buffer))//密码设置错误，程序退出
            {
                printf("Password setup wrong!!!\n");
                printf("%s", buffer);
                exit(EXIT_FAILURE);
            }
            else//密码设置正确
            {
//                memset(buffer, 0, sizeof(buffer));
                read(sock, buffer, sizeof(buffer));//服务器报告密码设置成功
                printf("%s", buffer);
                write(sock, buffer, sizeof(buffer));//考虑如何去掉这行代码
                read(sock, buffer, sizeof(buffer));//操作信息
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
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }
}

