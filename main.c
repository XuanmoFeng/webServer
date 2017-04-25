/*************************************************************************
> File Name: main.c
> Author: FENGXUANMO
> Mail: 763608087@qq.com
> Created Time: Tue 04 Apr 2017 01:07:59 AM PDT
************************************************************************/

#include"thttp.h"
int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("usepage:%s [local_ip][local_port]\n",argv[0]);
        return -1;
    }
    int listensock =StartUp(argv[1],argv[2]);
    struct sockaddr_in client;
    client.sin_family= AF_INET;
    socklen_t len =sizeof(client);
    int sock=0;
    while(1)
    {
        sock=accept(listensock,(struct sockaddr*)&client,&len);
        if(sock<0)
        {
            print_log("accept faile",WARING);
            exit(4);
        }
        pthread_t pid;
        pthread_create(&pid,NULL,phandler,(void *)sock);
    }
}
