/*************************************************************************
	> File Name:    test.cpp
	> Author:       spider
	> Mail:         13953028063@139.com
	> Created Time: 2018年11月23日 星期五 19时19分04秒
 ************************************************************************/

#include <QApplication>
#include <QWidget>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

class MyWidget:public QWidget
{
    public:
        MyWidget()
        ~MyWidget();
        InitUI();
        OnSignalConnectSlot();
    private:
        QCheckBox *checkBox;
        QGroupBox *gropBox;
        QRadioButton *radirBox;
        QVBoxLayout *VLayout;
        QHBoxLayout *HLayout;
        QTextEdit *textEdit;
}
