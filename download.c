#include "include.h"

#define DL_PATH "/home/ssu/C/socket/file/"

void download(struct sockaddr_in serv_addr, int sock)
{
    char bufRecv[BUF_SIZE] = {0};//文件缓冲区
    int nCount;
    char filename[BUF_SIZE] = {0};//用来保存文件名
    char download[1] = {'2'};//操作符，告知服务器进行什么操作

    write(sock, download, sizeof(download));//将操作符发送给服务器
    read(sock, bufRecv, BUF_SIZE);//接收从服务器反馈信息（可下载文件列表）
    printf("\nThe list of file that you can download:\n%s\n", bufRecv);
    printf("\nWhich file you want to download: ");
    scanf("%s", filename);
    write(sock, filename, sizeof(filename));//将想下载的文件名发给服务器

    char filepath[BUF_SIZE];//保存文件路径
    strcpy(filepath, DL_PATH);
    strcat(filepath, filename);
    
    FILE *fp = fopen(filepath, "wb");
    if(fp == NULL)
    {
        printf("Cannot open file!!!!\n");
    }
    //循环接受数据，直到文件传输完毕
    while((nCount = read(sock, bufRecv, BUF_SIZE)) > 0)
    {
        fwrite(bufRecv, nCount, 1, fp);
    }
    printf("File transfer success!\n");
    fclose(fp);
    //文件接受完毕后直接关闭套接字，无须调用shutdown
    close(sock);
}

