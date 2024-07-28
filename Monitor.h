#ifndef MONITOR_H
#define MONITOR_H

#include <cstddef>

class Monitor 
{
public:
    virtual void monitor() = 0;
    virtual ~Monitor() = default;
};

#endif // MONITOR_H
