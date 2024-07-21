#ifndef CONSOLEMONITOR_H
#define CONSOLEMONITOR_H

#include "Monitor.h"
#include <iostream>
#include <iomanip>
#include <sstream>

class ConsoleMonitor : public Monitor 
{
public:
    void monitor(const unsigned char* packet, std::size_t length) override 
    {
        std::ostringstream oss;
        for (std::size_t i = 0; i < length; ++i) {
            if (i > 0 && i % 16 == 0) {
                oss << std::endl;
            }
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet[i]) << ' ';
        }
        std::cout << oss.str() << std::endl;
    }
};

#endif // CONSOLEMONITOR_H
