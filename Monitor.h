#ifndef MONITOR_H
#define MONITOR_H

#include <cstddef>

class Monitor 
{
public:
    virtual void monitor(const unsigned char* packet, std::size_t length) = 0;
    virtual ~Monitor() = default;
};

#endif // MONITOR_H
