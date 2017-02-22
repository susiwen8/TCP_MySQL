#include "include.h"

void sendMessage(struct sockaddr_in serv_addr, int sock)
{
    char bufSend[BUF_SIZE] = {0};
    char bufRecv[BUF_SIZE] = {0};
    char Message[1] = {'1'};
    //创建套接字
    //server的close()不仅会关闭服务器端的 socket，
    //还会通知客户端连接已断开，客户端也会清理 socket 相关资源，
    //所以 client.cpp 中需要将 socket() 放在 while 循环内部，
    //因为每次请求完毕都会清理 socket，下次发起请求时需要重新创建
    
    //向服务器发起请求，IP地址和端口号保存在sockaddr_in结构体中
    write(sock, Message, sizeof(Message));//告知server，client进行什么操作
    //向服务器传送数据
    printf("Input message:");
    scanf("%s", bufSend);
    write(sock, bufSend, sizeof(bufSend));
    
    //读取服务器传回的数据
    read(sock, bufRecv, sizeof(bufRecv));
    printf("Message form server: %s\n", bufRecv);
    memset(bufSend, 0, BUF_SIZE);
    memset(bufRecv, 0, BUF_SIZE);
 
    //关闭套接字
    close(sock);
}
