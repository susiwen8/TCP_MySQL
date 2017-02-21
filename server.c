#include "include.h"

int main()
{
    char request_name[] = "User name:";
    char request_password[] = "Password:";
    char frozen[] = "This account is frozen!!!!\n"; 
    char warning[] = "Wrong Password!!!\n";
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

    while(1)
    {
        //accept()接收客户端请求
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, 
                            &clnt_addr_size);
        read(clnt_sock, id_client, sizeof(id_client));//获取client线程ID
        printf("\nClient %s has connected!\n", id_client);
        /* 连接数据库，看该客户是否有帐号, 若有要求客户输入密码，有三次机会，
         * 没有，询问是否需要创建账户，将账户名和密码添加到数据库*/
        write(clnt_sock, request_name, sizeof(request_name));//询问用户名
        read(clnt_sock, buffer, sizeof(buffer));//接收用户名
        char *pw = mysql(buffer);//从MySQL中获取该用户的密码用来密码验证
        memset(buffer, 0, sizeof(buffer));
        
        write(clnt_sock, request_password, sizeof(request_password));//询问密码
        int i = 0;
        int count = 3;//有三次输入密码机会
        while(i < 3)
        {
            read(clnt_sock, buffer, sizeof(buffer));//接收密码
            count--;
            if(0 == strcmp(pw, buffer))
            {
                char correct[] = WELCOME_MESSAGE;
                write(clnt_sock, correct, sizeof(correct));//密码正确，跳出while循环
                break;
            }
            else
            {
                if(0 == count)
                {
                    write(clnt_sock, frozen, sizeof(frozen));//三次密码都错误冻结账户
                    /*****加入一个Python发邮件提醒程序****/
                }
                else
                {
                    write(clnt_sock, warning, sizeof(warning));//向客户端发送密码错误信息
                }
            }
            i++;
        }
        
        memset(id_client, 0, sizeof(id_client));//清空id_client，理解为什么？？？
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
    close(serv_sock);
    return 0;
}
