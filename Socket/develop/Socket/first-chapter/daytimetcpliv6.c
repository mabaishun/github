#include <unp.h> //自定义头文件，包含了大部分网络程序都需要的许多系统头文件，并定义了所用到的各种常值

int main(int argc,char **argv)
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in6 servaddr;//网际套接字结构

    if (argc != 2)
        err_quit("usage:a.out <IPAddress>");//以err_开头的函数定义在unp库中
    if( (sockfd = socket(AF_INET6,SOCK_STREAM,0)) < 0 )
        err_sys("sock error!");
    bzero(&servaddr,sizeof(servaddr));//bzero作用：清零
    servaddr.sin6_family = AF_INET6;//地址族
    servaddr.sin6_port = htons(13);//端口号，htons：主机到网络短整数
    if(inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <=0 )//inet_pton:呈现形式到数值，即点分十进制到网络能识别的数值转换
        err_quit("inet_pton error for %s",argv[1]);
    //SA，在头文件unp中#define struct sockaddr SA
    if( connect(sockfd,(SA*)&servaddr,sizeof(servaddr)) < 0 )
        err_sys("connect error!");

    while( (n = read(sockfd, recvline,MAXLINE)) > 0 )//读取应答，read返回0表示对端关闭连接，返回-1表示错误
    {
        recvline[n] = 0;
        if(fputs(recvline, stdout) == EOF)//输出结果
            err_sys("fputs error!");
    }
    if(n < 0)
        err_sys("read error!");

    return 0;
}

int Socket(int family, int type, int protocol)
{
    int n;
    if( (n = socket(family,type,protocol))< 0 )
    {
        err-sys("socket error!")
    }
    return (n);
}
