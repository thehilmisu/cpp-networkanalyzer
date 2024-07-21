#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#include "Monitor.h"
#include "PacketParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>

class FileMonitor : public Monitor {
public:
    FileMonitor(const std::string& filename)
        : filename(filename), filePosition(0) {}

    void setPacketFilter(const std::string& srcIp, const std::string& dstIp);
    void monitor(const unsigned char* packet, std::size_t length) override ;
    void process(); 

private:
    void processBuffer(const unsigned char* buffer, std::size_t length);

    std::string filename;
    std::streampos filePosition;
    PacketParser parser;
};

#endif // FILEMONITOR_H
