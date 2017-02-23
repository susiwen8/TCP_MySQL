#include "include.h"

int main()
{
    //创建套接字
    //socket()函数确定了套接字的各种属性
    //AF_INET表示使用IPv4
    //SOCK_STREAM表示使用面向连接的数据传输方式
    //IPPROT_TCP表示使用TCP协议
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    
    //将套接字serv_sock与特定的IP地址和端口绑定，
    //IP地址和端口都保存在sockaddr_in结构体中
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    printf("Waiting client.......\n");
    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);
    
    char operation[1] = {0};
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    char id_client[BUF_SIZE] = {0};
    char buffer[BUF_SIZE] = {0};
    char yes_no[BUF_SIZE] = {0};
    const char verify[] = VERIFY;
    while(1)
    {
        //accept()接收客户端请求
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, 
                            &clnt_addr_size);
        memset(id_client, 0, sizeof(id_client));//理解为什么？？？
        read(clnt_sock, id_client, sizeof(id_client));//获取client线程ID
        printf("\nClient %s has connected!\n", id_client);
        write(clnt_sock, verify, sizeof(verify));//询问是否有账户
        read(clnt_sock, yes_no, sizeof(yes_no));//接收客户端的回答
        if(0 == strcmp(yes_no, YES))//有帐号的情况
        {
            code(clnt_sock);//密码验证
        //    memset(id_client, 0, sizeof(id_client));//理解为什么？？？
            read(clnt_sock, operation, sizeof(operation));//接收client操作选项
            switch(atoi(operation))
            {
                case 1:
                    {
                        printf("Client wants rebound message\n");
                        rebound(clnt_sock);
                        printf("\nClient has disconnected!\n");    
                        break;
                    }
                case 2:
                    {
                        printf("Client wants download file\n");
                        sendFile(clnt_sock);
                        printf("\nClient has disconnected!\n");    
                        break;
                    }
                default:
                    break;
            }
        }
        else
        {
            const char create_account[] = CREATE_ACCOUNT;
            write(clnt_sock, create_account, sizeof(create_account));
            read(clnt_sock, yes_no, sizeof(yes_no));
            if(0 == strcmp(yes_no, YES))
            {
                const char entre_name[] = ENTRE_NAME;
                write(clnt_sock, entre_name, sizeof(entre_name));
                read(clnt_sock, buffer, sizeof(buffer));
                char *pw = new_account_password(clnt_sock);
                MYSQL *conn = mysql_init(NULL);
                add_user(clnt_sock, buffer, pw);
                read(clnt_sock, buffer, sizeof(buffer));
                const char welcome[] = WELCOME_MESSAGE;
                write(clnt_sock, welcome, sizeof(welcome));
                read(clnt_sock, operation, sizeof(operation));//接收client操作选项
                switch(atoi(operation))
                {
                    case 1:
                        {
                            printf("Client wants rebound message\n");
                            rebound(clnt_sock);
                            printf("\nClient has disconnected!\n");    
                            break;
                        }
                    case 2:
                        {
                            printf("Client wants download file\n");
                            sendFile(clnt_sock);
                            printf("\nClient has disconnected!\n");    
                            break;
                        }
                    default:
                        break;
                }
            }
            else
            {
                printf("Client doesn't want to create an account!!!\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    close(serv_sock);
    return 0;
}
