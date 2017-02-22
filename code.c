#include "include.h"

void code(int clnt_sock)
{
    char buffer[BUF_SIZE] = {0};
    const char request_name[] = "User name:";
    const char request_password[] = "Password:";
    const char frozen[] = "This account is frozen!!!\n";
    const char warning[] = "Wrong password!!!\n";
    const char correct[] = WELCOME_MESSAGE;
    /* 连接数据库，看该客户是否有帐号, 若有要求客户输入密码，有三次机会，
     * 没有，询问是否需要创建账户，将账户名和密码添加到数据库*/
    write(clnt_sock, request_name, sizeof(request_name));//询问用户名
    read(clnt_sock, buffer, sizeof(buffer));//接收用户名
    char *pw = mysql(buffer, clnt_sock);//从MySQL中获取该用户的密码用来密码验证
    memset(buffer, 0, sizeof(buffer));
        
    write(clnt_sock, request_password, sizeof(request_password));//询问密码
    int i = 0;
    int count = 3;//有三次输入密码机会
    while(i < 3)
    {
        count--;
        read(clnt_sock, buffer, sizeof(buffer));//接收密码
        if(0 == strcmp(pw, buffer))
        {
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
        memset(buffer, 0, sizeof(buffer));
        i++;
    }
}
        
