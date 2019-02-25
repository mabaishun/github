#include <unp.h>
#include <time.h>
int main()
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    time_t ticks;

    listenfd = Socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = ntohs(13);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

    //调用listen函数把该套接字转换成一个监听套接字，这样来自客户的外来连接就可在该套接字上由内核接受
    Listen(listenfd, LISTENQ);

    for ( ; ; )
    {
        //在这里，服务器进程在accept调用中被投入睡眠，等待某个客户连接的到达并被内核接受，TCP连接使用所谓的三路握手来建立连接，握手完毕，accept返回，返回一个“已连接描述符”;该描述符用于与新近连接的那个客户通信，accept为每个连接到本服务器的客户返回一个新描述符
        connfd = Accept(listenfd,(SA*)NULL, NULL);

        ticks = time(NULL);
        snprintf(buf, sizeof(buf),"%.24s\r\n",ctime(&ticks));
        Write(connfd, buf, sizeof(buf));
        //通过调用close关闭与客户端的连接，该调用引发正常的TCP连接终止序列，每个方向上发送一个FIN，每个FIN又由各自的对端确认。
        Close(connfd);
    }
    return 0;
}

