/*************************************************************************
	> File Name:    goose.c
	> Author:       spider
	> Mail:         13953028063@139.com
	> Test platform:
	>               gcc:Ubuntu 5.4.0-6ubuntu1~16.04.10
	>               GNU bash, 4.15.0(39)-release (x86_64-pc-linux-gnu)
	> Created Time: 2018年11月26日 星期一 09时21分03秒
 ************************************************************************/
#include "goose.h"

#include <stdlib.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

char *pduName[11] = {
                "gocbRef          ",
                "timeAllowedtoLive",
                "datSet           ",
                "goID             ",
                "eventTimestamp   ",
                "StateNumber      ",
                "SequenceNumber   ",
                "test             ",
                "confRev          ",
                "ndsCom           ",
                "numDatSetEntries "
};
//一个释放内存的函数，在结构体的第一位，用来释放结构体的内存
void free_goose_manager(void *pm)
{
    int i = 0;
    if (pm != NULL)
    {
        
        goose *gs = (goose*)pm;
        
        if (gs->GOOSEEPDU.date[i].date != NULL)
        {
            while (i < 11)
            {
                free(gs->GOOSEEPDU.date[i].date);
                //gs->GOOSEEPDU.date[i].date = NULL;
                //printf("DATE FREE SUCCESS....\n");
                i++;
            }
        }
        /*
        while(i < 11)
        {
            if (gs->GOOSEEPDU.date[i].date != NULL)
            {
                free(gs->GOOSEEPDU.date[i].date);
            }
        }*/
        bit_string bstr[4] = {gs->GOOSEEPDU.allData.items2,gs->GOOSEEPDU.allData.items4,gs->GOOSEEPDU.allData.items6,gs->GOOSEEPDU.allData.items8};
        for (i = 0;i < 4 && bstr[i].bit_string != NULL;i++)
        {
            free(bstr[i].bit_string);
            //bstr[i].bit_string = NULL;
            //printf("BIT_STRING FREE SUCCESS....\n");
        }
        free(pm);
        pm = NULL;
    }
}
//根据传过来的包(buffer)判断是否是该协议，并且返回协议长度
unsigned int GooseLen(unsigned char *buffer,unsigned int buffLen)
{
    if (buffer == NULL)
    {
        printf("没有数据\n");
        return -1;
    }
    struct ether_header *eth;
    eth = (struct ether_header*)buffer;
    if (0x88b8 == ntohs(eth->ether_type))
    {
        /*
        buffer = buffer + sizeof(struct ether_header) - 8;
        goose *gs = (goose*)buffer;*/
        return buffLen - sizeof(struct ether_header);
    }
    else
    {
        return 0;
    }
}

//根据传过来的包(buffer)，解析，并且将解析结果填充到structBuff(是个指向goose的结构体的指针)
int analysisToStruct(void *structBuff,unsigned char *buffer,unsigned int buffLen)
{
    int err = 0;
    int offse = 0;
    int i = 0;
    goose * gs = (goose*)structBuff;
    buffer += sizeof(struct ether_header);
    u_int16* buff = (u_int16*)buffer;

    gs->GOOSEHEAD.AppID = ntohs(*buff);
    buff++;
    offse += 2;
    gs->GOOSEHEAD.Length = ntohs(*buff);
    buff++;
    offse += 2;
    gs->GOOSEHEAD.Reserverd1 = ntohs(*buff);
    buff++;
    offse += 2;
    gs->GOOSEHEAD.Reserverd2 = ntohs(*buff);
    buff++;
    offse += 2;
    u_int8* str = (u_int8*)buff;
    gs->GOOSEHEAD.tag = *str;
    str++;
    offse++;
    gs->GOOSEHEAD.len = *str;
    str++;
    offse++;
    gs->GOOSEHEAD.value = *str;
    str++;
    offse++;
    while(i < 11)
    {
        gs->GOOSEEPDU.date[i].tag = *str;
        str++;
        offse++;
        gs->GOOSEEPDU.date[i].value = *str;
        str++;
        offse++;

        gs->GOOSEEPDU.date[i].date = (u_int8*)malloc(gs->GOOSEEPDU.date[i].value);
        if (!gs->GOOSEEPDU.date[i].date)
        {
            perror("malloc");  
            printf("%s:%d\n",__func__,__LINE__);
            err = 1;
            goto load;
        }
        for (int j = 0;j < gs->GOOSEEPDU.date[i].value;j++)
        {
            *(gs->GOOSEEPDU.date[i].date + j) = *str;
            str++;
        }
        offse += gs->GOOSEEPDU.date[i].value;
        i++;
    }
    str += 2;
    offse += 2;
    gs->GOOSEEPDU.allData.items1.tag = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items1.len = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items1.boolean = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items2.tag = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items2.len = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items2.Padding = *str;
    str++;
    offse++;
    int len = gs->GOOSEEPDU.allData.items2.len;
    gs->GOOSEEPDU.allData.items2.bit_string = (u_int8*)malloc(len - 1);
    if (!gs->GOOSEEPDU.allData.items2.bit_string)
    {
        perror("malloc");
        printf("%s:%d\n",__func__,__LINE__);
        err = 2;
        goto load;
    }
    int j  = 0;
    while(j < len - 1)
    {
        *(gs->GOOSEEPDU.allData.items2.bit_string + j) = *str;
        str++;
        offse++;
        //(bstr[i].bit_string)++;
        j++;
    }


    gs->GOOSEEPDU.allData.items3.tag = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items3.len = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items3.boolean = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items4.tag = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items4.len = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items4.Padding = *str;
    str++;
    offse++;
    len = gs->GOOSEEPDU.allData.items4.len;
    gs->GOOSEEPDU.allData.items4.bit_string = (u_int8*)malloc(len - 1);
    if (!gs->GOOSEEPDU.allData.items4.bit_string)
    {
        perror("malloc");
        printf("%s:%d\n",__func__,__LINE__);
        err = 3;
        goto load;
    }
    j  = 0;
    while(j < len - 1)
    {
        *(gs->GOOSEEPDU.allData.items4.bit_string + j) = *str;
        str++;
        offse++;
        //(bstr[i].bit_string)++;
        j++;
    }


    gs->GOOSEEPDU.allData.items5.tag = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items5.len = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items5.boolean = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items6.tag = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items6.len = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items6.Padding = *str;
    str++;
    offse++;
    len = gs->GOOSEEPDU.allData.items6.len;
    gs->GOOSEEPDU.allData.items6.bit_string = (u_int8*)malloc(len - 1);
    if (!gs->GOOSEEPDU.allData.items6.bit_string)
    {
        perror("malloc");
        printf("%s:%d\n",__func__,__LINE__);
        err = 4;
        goto load;
    }
    j  = 0;
    while(j < len - 1)
    {
        *(gs->GOOSEEPDU.allData.items6.bit_string + j) = *str;
        str++;
        offse++;
        //(bstr[i].bit_string)++;
        j++;
    }


    gs->GOOSEEPDU.allData.items7.tag = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items7.len = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items7.boolean = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items8.tag = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items8.len = *str;
    str++;
    offse++;
    gs->GOOSEEPDU.allData.items8.Padding = *str;
    str++;
    offse++;
    len = gs->GOOSEEPDU.allData.items8.len;
    gs->GOOSEEPDU.allData.items8.bit_string = (u_int8*)malloc(len - 1);
    if (!gs->GOOSEEPDU.allData.items8.bit_string)
    {
        perror("malloc");
        printf("%s:%d\n",__func__,__LINE__);
        err = 5;
        goto load;
    }
    j  = 0;
    while(j < len - 1)
    {
        *(gs->GOOSEEPDU.allData.items8.bit_string + j) = *str;
        str++;
        offse++;
        //(bstr[i].bit_string)++;
        j++;
    }
    /*
    boolean bln[4] = {gs->GOOSEEPDU.allData.items1,gs->GOOSEEPDU.allData.items3,gs->GOOSEEPDU.allData.items5,gs->GOOSEEPDU.allData.items7};
    bit_string bstr[4] = {gs->GOOSEEPDU.allData.items2,gs->GOOSEEPDU.allData.items4,gs->GOOSEEPDU.allData.items6,gs->GOOSEEPDU.allData.items8};
    i = 0; 
    while(i < 4)
    {
        bln[i].tag = *str;
        str++;
        offse++;
        bln[i].len = *str;
        str++;
        offse++;
        bln[i].boolean = *str;
        str++;
        offse++;
        bstr[i].tag = *str;
        str++;
        offse++;
        bstr[i].len = *str;
        str++;
        offse++;
        bstr[i].Padding = *str;
        str++;
        offse++;
        bstr[i].bit_string = (u_int8*)malloc(bstr[i].len);
        int j  = 0;
        while(j < bstr[i].len - 1)
        {
            *(bstr[i].bit_string + j) = *str;
            str++;
            offse++;
            //(bstr[i].bit_string)++;
            j++;
        }
        
        // *(bstr[i].bit_string) = *str;
        //str++;
        //offse++;
        //(bstr[i].bit_string)++;
        // *(bstr[i].bit_string) = *str;
        //str++;
        //offse++;
        i++;
    }*/

    if (offse == gs->GOOSEHEAD.Length)
    {
        //printf("%d\n",offse);
        return 1;
    }
    else
    {
        //printf("%d\n",offse);
        return 0;
    } 

load:
    if (err == 1)
    {
        free(gs->GOOSEEPDU.date[i].date);
        return 0;
    }
    if (err == 2)
    {
        free(gs->GOOSEEPDU.allData.items2.bit_string);
        return 0;
    }
    if (err == 3)
    {
        free(gs->GOOSEEPDU.allData.items4.bit_string);
        return 0;
    }
    if (err == 4)
    {
        free(gs->GOOSEEPDU.allData.items6.bit_string);
        return 0;
    }
    if (err == 5)
    {
        free(gs->GOOSEEPDU.allData.items8.bit_string);
        return 0;
    }
}

//根据已经填充完的ptr里的数据(ptr指向goose结构体)，生成日志，并且写到gooseFile里
unsigned int structToFile(void *ptr,unsigned char *gooseFile,unsigned int buffLen)
{
    goose *gs = (goose*)ptr;
    int offse = 0;
    offse = sprintf(gooseFile,"####################################GOOSE协议数据###################################\n");
    gooseFile += offse;
    offse = sprintf(gooseFile,"APPID:           %04x\n",gs->GOOSEHEAD.AppID);
    gooseFile += offse;
    offse = sprintf(gooseFile,"Length:          %u\n",gs->GOOSEHEAD.Length);
    gooseFile += offse;
        //printf("%0x\n",gs->GOOSEEPDU.allData.items1.tag);
    int i = 0;
    while(i < 11)
    {
        offse = sprintf(gooseFile,"%s:TAG:  %02x",pduName[i],gs->GOOSEEPDU.date[i].tag);
        gooseFile += offse;
        offse = sprintf(gooseFile,"  LEN:  %02x",gs->GOOSEEPDU.date[i].value);
        gooseFile += offse;
        offse = sprintf(gooseFile," DATE:");
        gooseFile += offse;
        for (int j = 0;j <= gs->GOOSEEPDU.date[i].value - 1;j++)
        {
            offse = sprintf(gooseFile," %02x",*(gs->GOOSEEPDU.date[i].date + j));
            //(gs->GOOSEEPDU.date[i].date)++;
            gooseFile += offse;
        }
        offse = sprintf(gooseFile,"\n");
        gooseFile += offse;
        i++;
    }
    boolean bln[4] = {gs->GOOSEEPDU.allData.items1,gs->GOOSEEPDU.allData.items3,gs->GOOSEEPDU.allData.items5,gs->GOOSEEPDU.allData.items7};
    bit_string bstr[4] = {gs->GOOSEEPDU.allData.items2,gs->GOOSEEPDU.allData.items4,gs->GOOSEEPDU.allData.items6,gs->GOOSEEPDU.allData.items8};
    i = 0;
    offse = sprintf(gooseFile,"DATA:8 items\n");
    gooseFile += offse;
    while(i < 4)
    {
        offse = sprintf(gooseFile,"  DATA : boolean:\n");
        gooseFile += offse;
        offse = sprintf(gooseFile,"    boolean:%s\n",bln[i].boolean == 0 ? "false" : "true");
        gooseFile += offse;
        offse = sprintf(gooseFile,"  DATA : bit_string:\n");
        gooseFile += offse;
        offse = sprintf(gooseFile,"    Padding: %d\n",bstr[i].Padding);
        gooseFile += offse;
        offse = sprintf(gooseFile,"    bit_string:");
        gooseFile += offse;
        for (int j = 0;j < bstr[i].len - 1;j++)
        {
            offse = sprintf(gooseFile," %02x",*(bstr[i].bit_string + j));
            gooseFile += offse;
        }
        offse = sprintf(gooseFile,"\n");
        gooseFile += offse;
        i++;
    }
    offse = sprintf(gooseFile,"##############################################GOOSE数据解析完成#######################################\n");
    gooseFile += offse;
    return 1;
}
