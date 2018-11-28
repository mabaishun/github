/*************************************************************************
	> File Name:    goose.h
	> Author:       spider
	> Mail:         13953028063@139.com
	> Test platform:
	>               gcc:Ubuntu 5.4.0-6ubuntu1~16.04.10
	>               GNU bash, 4.15.0(39)-release (x86_64-pc-linux-gnu)
	> Created Time: 2018年11月26日 星期一 09时21分03秒
 ************************************************************************/

#ifndef _GOOSE_H
#define _GOOSE_H
#include <stdio.h>
#include <string.h>
typedef unsigned char u_int8;
typedef unsigned short u_int16;
typedef unsigned int u_int32;

typedef struct
{
    u_int8 tag;
    u_int8 len;
    u_int8 boolean;
}boolean;
typedef struct
{
    u_int8 tag;
    u_int8 len;
    u_int8 Padding;
    u_int8 *bit_string;
}bit_string;
typedef struct
{
    u_int8 tag;
    u_int8 value;
    u_int8 *date;
}goosedate;
typedef struct{
    void (*free_manager)(void *pm);
    struct GOOSEHEAD{
        u_int16 AppID;//0x0000~0x3fff，其值来源于GOOSE配置文本中目的地址中的Appid
        u_int16 Length;//从AppID开始计数到PDU结束的全部字节长度
        u_int16 Reserverd1;//保留字
        u_int16 Reserverd2;//保留字
        u_int8 tag;
        u_int8 len;
        u_int8 value;
    }GOOSEHEAD;
    struct {
        goosedate date[11]; 
        /*
        u_int8 *gocbRef;//控制块引用名
        u_int8 *timeAllowedtoLive;//生存周期
        u_int8 *datSet;//数据集引用名
        u_int8 *goID;//控制块ID
        u_int8 *eventTimestamp;//时间时标
        u_int8 *StateNumber;//状态号，0～4294967295,从0开始，没产生一次变化数据，该值加1
        u_int8 *SequenceNumber;//序号，0～4294967295，从0开始，每产生一次变化数据，该值加1
        u_int8 *test;//检修标识
        u_int8 *confRev;//配置版本
        u_int8 *ndsCom;//暂时未使用
        u_int8 *numDatSetEntries;//数据集条目数，控制对应的数据集中的条目数
        */
        struct{
            //数据集中每个数据的实时值
            boolean items1;
            bit_string items2;
            boolean items3;
            bit_string items4;
            boolean items5;
            bit_string items6;
            boolean items7;
            bit_string items8;
        }allData;

    }GOOSEEPDU;
}goose;

//一个释放内存的函数，在结构体的第一位，用来释放结构体的内存
void free_goose_manager(void *pm);
//根据传过来的包(buffer)判断是否是该协议，并且返回协议长度
unsigned int GooseLen(unsigned char *buffer,unsigned int bufLen);

//根据传过来的包(buffer)，解析，并且将解析结果填充到structBuff(是个指向goose的结构体的指针)
int analysisToStruct(void *structBuff,unsigned char *buffer,unsigned int bufLen);

//根据已经填充完的ptr里的数据(ptr指向goose结构体)，生成日志，并且写到gooseFile里
unsigned int structToFile(void *ptr,unsigned char *gooseFile,unsigned int bufLen);



#endif
