#ifndef COMMON_H
#define COMMON_H

struct RawData
{
    RawData()
    {
        time = 0;
        size = 0;
        count = 0;
    }

    int time;
    int size;
    int count;
};

#endif // COMMON_H
