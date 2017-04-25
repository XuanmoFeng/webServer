/*************************************************************************
	> File Name: thttp.h
	> Author: FENGXUANMO
	> Mail: 763608087@qq.com
	> Created Time: Tue 04 Apr 2017 12:46:16 AM PDT
 ************************************************************************/

#ifndef _THTTP_H
#define _THTTP_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#define NORMAL 0
#define WARING 1
#define FATAL  2
#define SIZE 1024
int StartUp(char *ip,char *port);
void print_log(const char *mess,int lev);
void *phandler(void* sock);
void do_cgi(int sock ,char *mothod,char *p_string,char*path);
int FindPath(int sock,char *path,char *mothod,int*cgi);
#endif
