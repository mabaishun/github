/*-----------------------------------------------------------------------------
 * | 异常                                                                      
 * |  常见错误                                                                
 * |   1 语法错误                                                             
 * |   2 逻辑错误                                                             
 * |   3 功能错误                                                             
 * |   4 设计缺陷                                                             
 * |   5 需求不符                                                             
 * |   6 环境异常                                                             
 * |   7 操作不当
 * |  C语言的错误处理机制
 * |    1 通过返回值表示错误
 * |      优点：函数调用路径中所有的局部对象都能被正常的析构，不会内存泄露
 * |      缺点：错误处理流程比较复杂，逐层判断，代码臃肿
 * |    2 通过远程跳转处理错误
 * |      优点：不需要逐层判断，实现一步到位的错误处理，代码精炼
 * |        #include<setjmp.h>      longjmp()   setjmp()
 * |      缺点：函数调用路径中局部对象失去被析构的机会，形成内存泄露
 * |  C++异常机制
 * |    结合C中两种异常处理优点，同时避免它们的缺点，在形式上实现一步到位的错误处理
 * |    ，无需逐层判断返回值，同时保证所有的局部对象得到正确的析构
 * ---------------------------------------------------------------------------*/
#include <iostream>
#include <cstdio>
#include <csetjmp>
using namespace std;

jmp_buf g_env;

class A{
public:
    A(void){cout << "A::A()" << endl;}
    ~A(void){cout << "A::~A()" << endl;}
};

int func3(void)
{
    A a;
    FILE *fp = fopen("none.txt", "r");
    if(fp == NULL)
    {
        longjmp(g_env,-1);
    }
    //....
    fclose(fp);
    return 0;
}

int func2(void)
{
    A a;
    func3();
    return 0;
}
int func1(void)
{
    A a;
    func2();
    return 0;
}
int main(void)
{
    if(setjmp(g_env) == -1)
    {
        cout << "file open error!" << endl;
        return -1;
    }
    func1();
    //....
    return 0;
}

