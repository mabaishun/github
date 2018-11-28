/*************************************************************************
	> File Name:    main.c
	> Author:       spider
	> Mail:         13953028063@139.com
	> Test platform:
	>               gcc:Ubuntu 5.4.0-6ubuntu1~16.04.10
	>               GNU bash, 4.15.0(39)-release (x86_64-pc-linux-gnu)
	> Created Time: 2018年11月27日 星期二 16时04分43秒
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "goose.h"



#define ETH_NAME    "ens33"
 

void readFile(void*,size_t);
int printFile(void *,size_t);
int fd;
#include <netinet/if_ether.h>
int main(void)
{
    int s;
    struct ifreq ifr;
    struct ether_header* eth;
    int sockfd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    if (sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    strcpy(ifr.ifr_name,ETH_NAME);
    if (s = ioctl(sockfd,SIOCGIFFLAGS,&ifr) < 0)
    {
        close(sockfd);
        return -1;
    }
    
    ifr.ifr_flags |= IFF_PROMISC;
    if (ifr.ifr_flags == IFF_RUNNING)
    {
        printf("eth link up!\n");
    }
    else
    {
        printf("eth link down!\n");
    }
    if (s = ioctl(sockfd,SIOCSIFFLAGS,&ifr) < 0)
    {
        return -1;
    }
    printf("Setting interface ::: %s ::: to promisc\n\n", ifr.ifr_name);
    
    fd = open("log.txt",O_RDWR | O_CREAT,0664);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    char *buffer = (char*)malloc(65536);
    if (!buffer)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int num = 0;
    int ret = 0;
    while(num < 20)
    {
        if((ret = read(sockfd,buffer,65536)) == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (!buffer)
        {
            continue;
        }
        if (buffer)
        {
            eth = (struct ether_header*)buffer;
            if (0x88b8 == ntohs(eth->ether_type))
            {
                readFile(buffer,ret);
            }
        }
        num++;
    }
    free(buffer);
    buffer = NULL;
    
    return 0;
}
void readFile(void *buffer,size_t size)
{
    
    int returnValue = GooseLen(buffer,size);
    if (returnValue)
    {
        printf("goose数据长度:   %d\n",returnValue);       
    }
    else
    {
        printf("数据错误\n");
        return;
    }
    goose* gs = (goose*)malloc(4096);
    if (!gs)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memset(gs,0,4096);
    if (analysisToStruct(gs,buffer,returnValue))
    {
        u_int8 *structFile = (u_int8*)malloc(4096);
        if (!structFile)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        structToFile(gs,structFile,size);
        *(structFile + strlen(structFile)) = '\0';
        printFile(structFile,4096);
        gs->free_manager = free_goose_manager;
        if (gs != NULL)
        {
            gs->free_manager(gs);
            gs = NULL;
        }
        if (structFile != NULL)
        {
            free(structFile);
            structFile = NULL;
        }
    }
    else
    {
        printf("%s:%d",__func__,__LINE__);
        gs->free_manager = free_goose_manager;
        if (gs != NULL)
        {
            gs->free_manager(gs);
            gs = NULL;
        }
        exit(EXIT_FAILURE);
    }
}

int printFile(void *p,size_t size)
{
    int ret = 0;
    if ((ret = write(fd,p,strlen(p))) == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    return 0;
}
