#include <QTime>

#include "calprointerval.h"

CalProInterval* CalProInterval::Instance = new CalProInterval();
CalProInterval::CalProInterval()
{
    start_ = QTime::currentTime();
    end_ = QTime::currentTime();
}

CalProInterval::~CalProInterval()
{}

double CalProInterval::getDiffTime(QTime start, QTime end)
{
    return 1000 * ((end.hour()-start.hour()) * 24 * 60 * 60 + (end.minute() - start.minute()) * 60 + (end.second() - start.second())) + (end.msec() - start.msec());
}

double CalProInterval::getDiffTime()
{
    updateEndTime();
    return 1000 * ((end_.hour()-start_.hour()) * 24 * 60 * 60 + (end_.minute() - start_.minute()) * 60 + (end_.second() - start_.second())) + (end_.msec() - start_.msec());
}

QTime CalProInterval::updateStartTime()
{
    start_ = QTime::currentTime();
    return start_;
}

QTime CalProInterval::updateEndTime()
{
    end_ = QTime::currentTime();
    return end_;
}
