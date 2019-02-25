/*--------------------------------------------------------------------------------
 * | 异常                                                                         |
 * |  常见错误                                                                    |
 * |   1 语法错误                                                                 |
 * |   2 逻辑错误                                                                 |
 * |   3 功能错误                                                                 |
 * |   4 设计缺陷                                                                 |
 * |   5 需求不符                                                                 |
 * |   6 环境异常                                                                 |
 * |   7 操作不当                                                                 |
 * |  C语言的错误处理机制                                                         |
 * |    1 通过返回值表示错误                                                      |
 * |      优点：函数调用路径中所有的局部对象都能被正常的析构，不会内存泄露        |
 * |      缺点：错误处理流程比较复杂，逐层判断，代码臃肿                          |
 * |    2 通过远程跳转处理错误                                                    |
 * |      优点：不需要逐层判断，实现一步到位的错误处理，代码精炼                  |
 * |        #include<setjmp.h>      longjmp()   setjmp()                          |
 * |      缺点：函数调用路径中局部对象失去被析构的机会，形成内存泄露              |
 * |  C++异常机制                                                                 |
 * |    结合C中两种异常处理优点，同时避免它们的缺点，在形式上实现一步到位的       |
 * |    错误处理                                                                  |
 * |    ，无需逐层判断返回值，同时保证所有的局部对象得到正确的析构                |
 * |  C++异常语法                                                                 |
 * |    1 异常抛出                                                                |
 * |      throw 异常对象;                                                         |
 * |    2 异常捕获                                                                |
 * |      try{                                                                    |
 * |        可能引发异常的语句;                                                   |
 * |      }                                                                       |
 * |      catch(异常类型1){                                                       |
 * |        针对异常类型1的处理;                                                  |
 * |      }                                                                       |
 * |      catch(异常类型2){                                                       |
 * |        针对异常类型1的处理;                                                  |
 * |      }                                                                       |
 * |      catch(...){                                                             |
 * |        针对其他异常类型的处理;                                               |
 * |      }                                                                       |
 * |    注：catch子句根据异常对象的类型自上而下顺序匹配，而不是最优匹配，         |
 * |        因此对子类类型的异常的捕获不能放在对基类类型的异常捕获后面，          |
 * |        否则子类异常捕获会被基类的异常捕获提前捕获                            |
 * |  异常说明                                                                    |
 * |    可以在函数原型中增加异常说明，说明该函数所可能抛出的异常类型              |
 * |    返回类型 函数名(形参表)[const] throw(异常类型表){...}                     |
 * |    eg:                                                                       |
 * |        //告诉编译器，func函数可能会抛出int或char *类型的异常                 |
 * |        void func(void)throw(int,const char*){                                |
 * |            throw 1; //ok   int    可以被catch捕获                            |
 * |            throw "error"; //ok   const char*    可以被catch捕获              |
 * |            throw FileError(); //ok   const char*    可以抛出无法被catch捕获  |
 * |        }                                                                     |
 * |    函数的异常说明，只是一种承诺，表示该函数所抛出的异常不会超出所说明的类型  |
 * |    范围。但如果抛出了说明以外的异常类型，该异常无法被catch捕获，而会被系统捕 |
 * |    终止进程。                                                                |
 * |    不写异常说明，表示可以抛出任何异常                                        |
 * |    空异常说明，throw(),表示不会抛出任何异常                                  |
 * |    如果函数的声明和定义分开写，要保证异常说明的类型一致，但是顺序无所谓      |
 * |    如果基类中徐函数带有异常说明，那么该函数在子类中的覆盖版本不能说明比      |
 * |    基类版本抛出更多的异常，否则因为"放松throw限定"导致编译失败               |
 * |  C++标准异常类                                                               |
 * |    class exception{                                                          |
 * |        public:                                                               |
 * |            exception()throw(){}                                              |
 * |            virtual ~exception() throw();                                     |
 * |            virtual const char* what() const throw();                         |
 * |    };                                                                        |
 * |  注：自定义异常类时，建议继承标准异常类，并重写what函数，让其对标准异常类中  |
 * |    的what形成覆盖，通过多态的语法特性，在子类的what函数处理子类的异常        |
 * |  构造函数中的异常                                                            |
 * |    构造函数抛出异常后，对象将会被不完整构造，这样的对象其析构函数不会被执行，|
 * |    因此在构造函数抛出异常之前，需要手动销毁所有在异常产生之前的动态资源.     |
 * |  析构函数最好不要抛出异常                                                    |
 * |                                                                              |
 * -------------------------------------------------------------------------------*/
#include <iostream>
#include <cstdio>
using namespace std;

class A{
public:
    A(void){cout << "A::A()" << endl;}
    ~A(void){ cout << "A::~A()" << endl; }
};
class B
{
    public:
        B(void):m_a(new A){
            FILE*fp = fopen("none.txt","r");
            if(!fp)
            {
                delete m_a;
                throw exception();
            }
            fclose(fp);
        }
        ~B(void)
        {
            delete m_a;
        }
    private:
        A* m_a;
};

int main(void)
{
    try
    {
        B b;
    }
    catch(exception&  ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    return 0;
}


#if 0
class FileError:public exception{
public:
    const char*what(void)const _GLIBCXX_USE_NOEXCEPT{
        return "File open error!";
    }
};

int main(void)
{
    try
    {
        throw FileError();
        char *parr = new char[0xffffffff];
    }
    catch(exception& ex)
    {
        cout << ex.what() << endl;
    }
    return 0;
}
#endif
