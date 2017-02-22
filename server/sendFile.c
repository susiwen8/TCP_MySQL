#include "include.h"

#define FILE_PATH "/home/ssu/Downloads/"

void sendFile(int clnt_sock)
{
    file(clnt_sock);
    
    char fileName[BUF_SIZE] = {0};
    read(clnt_sock, fileName, BUF_SIZE);//接收并读取要下载的文件名
    printf("\nThe client wants to download %s\n", fileName);
    char filePath[BUF_SIZE];
    strcpy(filePath, FILE_PATH);//下载文件所在的文件夹
    strcat(filePath, fileName);//下载文件的完整路径

    FILE *fp = fopen(filePath, "rb");
    if(fp == NULL)
    {
        printf("Cannot open file!!!\n");
    }

    //循环发送数据，直到文件结尾
    char file[BUF_SIZE] = {0};//缓冲区
    int nCount;
    while((nCount = fread(file, 1, BUF_SIZE, fp)) > 0)
    {
        write(clnt_sock, file, nCount);
    }
    shutdown(clnt_sock, SHUT_WR);//文件读取完毕，断开输出流，向客户段发送FIN包
    //read并没有接收到client端的数据，当client调用close后，server会收到FIN包
    //read就会返回，后面代码继续执行
    read(clnt_sock, file, BUF_SIZE);//阻塞，等待客户端接受完毕
    fclose(fp);
    close(clnt_sock);
}

//查看有哪些文件可以下载
//并向客户端回传可下载文件列表
void file(int clnt_sock)
{
    system("ls /home/ssu/Downloads > file.txt");
    char buffer[BUF_SIZE] = {0};
    FILE *fp = fopen("file.txt", "rb");
    if(fp == NULL)
    {
        printf("Open file.txt failed!!!\n");
    }
    int nCount;
    while((nCount = fread(buffer, 1, BUF_SIZE, fp)) > 0)
    {
        printf("%s\n", buffer);
        write(clnt_sock, buffer, nCount);
    }
    fclose(fp);
}
