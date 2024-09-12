#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#include "pcapinterpreter.h"
#include "IMonitor.h"
#include <chrono>
#include <thread>

class FileMonitor : public IMonitor
{

public:
    static FileMonitor& getInstance();
    void setFileName(const std::string& fileName);
    void setFilePosition(std::streampos position);
    void setPcapInterpreter(PcapInterpreter *pcapInterpreter);
    void requestStop();
    void run();

private:
    FileMonitor() = default;
    ~FileMonitor();
    FileMonitor(const FileMonitor&) = delete;
    FileMonitor& operator=(const FileMonitor&) = delete;

    void monitor() override;

    std::string m_Filename;
    std::streampos m_FilePosition;
    PcapInterpreter *m_Interpreter;

    bool stopRequested;
};

#endif // FILEMONITOR_H
