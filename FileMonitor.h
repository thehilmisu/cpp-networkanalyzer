#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#include "IMonitor.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include "PcapInterpreter.h"

class FileMonitor : public IMonitor 
{
public:
    FileMonitor(const std::string& filename) : m_Filename(filename), m_FilePosition(0) {}
    void monitor() override ;

private:
    std::string m_Filename;
    std::streampos m_FilePosition;
    PcapInterpreter m_Interpreter;
};

#endif // FILEMONITOR_H
