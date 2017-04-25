/*************************************************************************
> File Name: hello.c
> Author: FENGXUANMO
> Mail: 763608087@qq.com
> Created Time: Mon 10 Apr 2017 03:56:56 AM PDT
************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
    char *method;
    char *p_string;
    int Content_len;
    method=getenv("METHOD");
    if(strcasecmp("GET",method)==0)
    {
        p_string=getenv("P-STRING");   
    }
    else
    {
        char c;
        Content_len=atoi(getenv("CONTENT_LENGTH"));
        p_string=malloc(Content_len);
        int i=0;
        for(;i<Content_len;i++)
        {
            read(0,&c,1);
             p_string[i]=c;
        }
    }
    do_math(p_string);
    printf("<html>");
    printf("<head>");
    printf("hello</br>%s</br>Content_len=%d",method,Content_len);
    printf("</br>string=%s",p_string);
    printf("<h2>hello client</h2>");
    printf("</head>");
    printf("<body>");
    printf("<p></p>");
    printf("</body>");
    printf("</html>");
    return 0;
}
