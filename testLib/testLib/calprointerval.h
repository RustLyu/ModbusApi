#ifndef CALPROINTERVAL_H
#define CALPROINTERVAL_H

#include <QTime>

// 计算程序耗时(ms)
// 计算开始处:updateStartTime(), 结束处调用getDiffTime() 即可
class CalProInterval
{
private:
    CalProInterval();
    ~CalProInterval();
public:
    static CalProInterval* Instance;
private:
    QTime start_;
    QTime end_;
public:
    double getDiffTime(QTime start, QTime end);
    double getDiffTime();
    QTime updateStartTime();
    QTime updateEndTime();

};

#endif // CALPROINTERVAL_H
