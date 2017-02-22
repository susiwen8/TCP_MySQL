#include "include.h"

void rebound(int clnt_sock)
{
    char buffer[BUF_SIZE] = {0};
    read(clnt_sock, buffer, sizeof(buffer));
    printf("Message from client: %s", buffer);
    //向客户端发送数据
    write(clnt_sock, buffer, sizeof(buffer));
    //关闭套接字
    close(clnt_sock);
    memset(buffer, 0, BUF_SIZE);
}
