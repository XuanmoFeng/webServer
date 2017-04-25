/*************************************************************************
> File Name: thttp.c
> Author: FENGXUANMO
> Mail: 763608087@qq.com
> Created Time: Tue 04 Apr 2017 12:50:29 AM PDT
************************************************************************/

#include"thttp.h"

int StartUp(char *Ip,char *port)
{

    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        print_log("socket faile",WARING);
        exit(1);
    }
    int reuse=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr(Ip);
    server.sin_port=htons(atoi(port));
    socklen_t len=sizeof(server);

    if(bind(sock,(struct sockaddr*)&server,len)<0){
        print_log("bind faile",FATAL);
        exit(2);
    }
    if(listen(sock,5)<0){
        print_log("listen faile",FATAL);
        exit(3);  
    }
    return sock;
}
static void error404(int sock)
{
    const char* str = "HTTP/1.0 404 NOT FOUND\r\n"
                    "Server: wz simple httpd 1.0\r\n"
                    "Content-Type: text/html\r\n"
                    "\r\n";
    send(sock, str, strlen(str),0);
}
static void error401(int sock)
{
    const char* str = "HTTP/1.0 401 OK\r\n"
                    "Server: wz simple httpd 1.0\r\n"
                    "Content-Type: text/html\r\n"
                    "\r\n";
    send(sock, str, strlen(str),0);
}
static void error403(int sock)
{
    const char* str = "HTTP/1.0 403 OK\r\n"
                    "Server: wz simple httpd 1.0\r\n"
                    "Content-Type: text/html\r\n"
                    "\r\n";
    send(sock, str, strlen(str),0);
}
static void error500(int sock)
{
    const char* str = "HTTP/1.0 500 OK\r\n"
                    "Server: wz simple httpd 1.0\r\n"
                    "Content-Type: text/html\r\n"
                    "\r\n";
    send(sock, str, strlen(str),0);
}
ssize_t get_line(int sock,char *buf){
    char c[1];
    int i=0;
    ssize_t s=recv(sock,c,1,0);
    while(c[0]!='\n'){
        buf[i++]=c[0];
        recv(sock,c,1,0);
        if(c[0]=='\r'){
            recv(sock,c,1,MSG_PEEK);
            if(c[0]=='\n'){
                recv(sock,c,1,0);
            }
        } 
    }
    buf[i]='\n';
    return i;
}

void print_log(const char *str, int lenvel){
    const  char Message[5][10]={"NORMAL","WARING","FATAL"};
    printf("%s[%-10s]\n",str,Message[lenvel]);
}
static void echo_www(int sock,char *path,int size)
{
        int fd =open(path,O_RDONLY);
        if(fd<0){
        }

         const char* str = "HTTP/1.0 200 OK\r\n"
        "Server: wz simple httpd 1.0\r\n"
        "Content-Type: text/html\r\n"
        "\r\n";
        send(sock, str, strlen(str),0);
        sendfile(sock,fd,0,size);
        close(fd);
}
int FindPath(int sock,char *path,char *mothod,int*cgi){
    char b[80];
    getcwd(b,sizeof(b));
    strcat(b,path);
    strcpy(path,b);
    struct stat buf;
    int ret =stat(path,&buf);
    if(ret <0){
        error404(sock);
    }
    else if(S_ISDIR(buf.st_mode)){
        strcat(path,"index.html");
        struct stat st;
        if(stat(path,&st)<0)
        {
            error404(sock);
            return 10;
        }
        else{
            echo_www(sock,path,st.st_size);
        } 
    }else if((buf.st_mode&S_IXOTH)==1||(buf.st_mode&S_IXUSR==1)){
        *cgi=1;
    }
}
void clear_header(int sock ,char*buf){
    while(get_line(sock ,buf)!=1&&buf[0]!='\n');
    return ;
}
int do_hand(int listen){
    char buf[SIZE],mothod[20],Version[20],Url[SIZE],p_string[SIZE];
    int cgi =0;
    int i=0,j=0;
    get_line(listen,buf);
    while(buf[j]!=' ')//mothod
    mothod[i++]=buf[j++];
    mothod[i]='\0';
    j++;
    i=0;
    while(buf[j]!=' ')
        Url[i++]=buf[j++];
    Url[i]='\0';
    j++;
    i=0;
    while(buf[j]!='\n')
    {
        Version[i++]=buf[j++];
    }
    Version[i]='\0';
    i=0;
    if(strcasecmp("post",mothod)!=0&&strcasecmp("get",mothod)!=0)
    {
        error401(listen);//
        return 9;
    }
    if(strcasecmp("get",mothod)==0)
    {
           clear_header(listen,buf);
        i=0;
        while(Url[i]!='?'&&Url[i]!='\0')
            i++;
        j=0;
        while(Url[i]!='\0')
            p_string[j++]=Url[++i];
        Url[i-j]='\0';
        p_string[j]='\0';
        if(strlen(p_string)>0)
            cgi=1;
    }
    else if(strcasecmp("post",mothod)==0)
        cgi=1;
    FindPath(listen,Url,mothod,&cgi);
    if(cgi)
        do_cgi(listen,mothod,p_string,Url);   
    close(listen);
    return 0;
}
void  *phandler(void *sock)
{
    pthread_detach(pthread_self());
    int listen =(int)(int*)sock;
    do_hand(listen);
    return NULL;
}

void do_cgi(int sock ,char *mothod,char *p_string,char*path){
    int input[2],output[2];
    pipe(input);
    pipe(output);
    int status,len=0;
    char buf[SIZE];
    get_line(sock,buf);
    if(strcasecmp("post",mothod)==0){
        while(strncasecmp("content-length: ",buf,15)!=0){
            get_line(sock,buf);
        }
        len=atoi(&buf[16]);
        clear_header(sock,buf);
    }
    const char* str = "HTTP/1.0 200 OK\r\n"
    "Server: wz simple httpd 1.0\r\n"
    "Content-Type: text/html\r\n"
    "\r\n";
    send(sock,str,strlen(str),0);
    pid_t id =fork();
    if(id==0){
        char method_s[SIZE];
        char p_string_s[SIZE];
        char cont_len[SIZE];       
        dup2(input[0],0);//ru
        dup2(output[1],1);//chu
        close(output[0]);
        close(input[1]);
        sprintf(method_s,"METHOD=%s",mothod);
        putenv(method_s);//method 
        if(strcasecmp("get",mothod)==0){
            sprintf(p_string_s,"P-STRING=%s",p_string);
            putenv(p_string_s);
        }
        else{
            sprintf(cont_len,"CONTENT_LENGTH=%d",len);
            putenv(cont_len);//p_string
        }
        execl(path,path,NULL);
        exit(0);
    }
    else{//parent
        char c='\0';
        int i=0;
        close(output[1]);
        close(input[0]);
        if(strcasecmp("post",mothod)==0){
            for(i=0;i<len;i++){
                recv(sock,&c,1,0);
                write(input[1],&c,1);
            }
        }
        while(read(output[0],&c,1)>0){
            send(sock,&c,1,0);    
        }
        close(output[0]);
        close(input[1]);
        waitpid(id,&status,NULL);
    }
}
