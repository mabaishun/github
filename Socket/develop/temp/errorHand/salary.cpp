#include <iostream>
using namespace std;

class Employee
{
public:
    Employee(const string& name, int id, int grade):m_name(name),m_id(id),m_grade(grade){}
    void printInfo(void)
    {
        printBasic();//员工基本信息
        printExtra();//特有员工
    }
    void paySalary(void)
    {
       cout << "工资：" <<  calBasic() + calMerit() << endl;
    }
protected:
    double m_attend;//出勤率
private:
    //计算普通工资
    double calBasic(void)
    {
        cout << "输入出勤天数：";
        int attend;
        cin >> attend;
        //计算出勤率
        m_attend = attend/23.0;
        m_basic = s_grades[m_grade - 1]*m_attend;
        return m_basic;
    }
    //绩效工资，普通工资的一半
    virtual double calMerit(void)
    {
        return m_basic / 2;
    }
    //打印普通员工信息
    void printBasic(void)
    {
        cout << "姓名：" << m_name << endl;
        cout << "工号：" << m_id << endl;
        cout << "职位级别：" << m_grade << endl;
    }
    virtual void printExtra(void)
    {
        cout << "职位：普通员工" << endl;
    }
    string m_name;
    int m_id;
    int m_grade;
    double m_basic;
    static double s_grades[6];//薪资表
};

double Employee::s_grades[] = {3000,4000,5000,6000,7000,8000};

//经理
class Manager:virtual public Employee
{
public:
    Manager(const string& name, int id, int grade, double bonus):Employee(name,id,grade),m_bonus(bonus){}
protected:
    void printExtra(void)
    {
        cout << "职位：经理" << endl;
        cout << "绩效奖金：" << m_bonus << endl;
    }
    double calMerit(void)
    {
        cout << "输入绩效因数：";
        double factor;
        cin >> factor;
        return m_bonus * factor;
    }
private:
    double m_bonus;
};

//技术员
class Technician:virtual public Employee
{
public:
    Technician(const string& name, int id, int grade, double allow):Employee(name,id,grade),m_allow(allow){}
protected:
    void printExtra(void)
    {
        cout << "职位：技术员" << endl;
        cout << "研发津贴" << m_allow << "小时/元" << endl;
    }
    double calMerit(void)
    {
        cout << "输入进度因数：";
        double factor;
        cin >> factor;
        return 8 * 23 * m_attend * m_allow * factor;
    }
private:
    double m_allow;
};

//技术主管
class TechMngr:public Technician,public Manager
{
public:
    TechMngr(const string& name, int id, int grade, double bonus, double allow):Technician(name, id, grade, allow), Manager(name,id,grade,bonus),Employee(name, id, grade){}
private:
    void printExtra()
    {
        cout << "职位：技术主管" << endl;
        Technician::printExtra();
        Manager::printExtra();
    }
    double calMerit(void)
    {
        return (Technician::calMerit() + Manager::calMerit())/2;
    }
};

int main()
{
    Employee emp("张飞",10011,2);
    emp.printInfo();
    emp.paySalary();

    cout << "#############################################" << endl;
    Manager mngr("关于", 10010,4,6000);
    mngr.printInfo();
    mngr.paySalary();

    cout << "#############################################" << endl;
    Technician tech("孔明",10012,3,30);
    tech.printInfo();
    tech.paySalary();

    cout << "#############################################" << endl;
    TechMngr techmngr("刘备", 10000,5,6000, 30);
    techmngr.printInfo();
    techmngr.paySalary();
    return 0;
}
