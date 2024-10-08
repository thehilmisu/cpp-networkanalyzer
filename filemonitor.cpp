#include "filemonitor.h"
#include <fstream>
#include <iostream>

FileMonitor& FileMonitor::getInstance()
{
    static FileMonitor instance;
    return instance;
}


FileMonitor::~FileMonitor()
{
    // Ensure the thread is safely stopped before destruction
    if (stopRequested)
    {
        //quit();
        //wait();
    }
}

void FileMonitor::setPcapInterpreter(PcapInterpreter *pcapInterpreter)
{
    m_Interpreter = pcapInterpreter;
}

// Function to set the file name
void FileMonitor::setFileName(const std::string& fileName)
{
    m_Filename = fileName;
    stopRequested = false;
}

// Function to set the file position
void FileMonitor::setFilePosition(std::streampos position)
{
    m_FilePosition = position;
}

// The thread's run method
void FileMonitor::run()
{
    monitor();
}

void FileMonitor::requestStop()
{
    //QMutexLocker locker(&mutex);
    stopRequested = true;
}

// The monitoring function
void FileMonitor::monitor()
{
    setFilePosition(0);
    while (!stopRequested)
    {
        std::ifstream fileStream(m_Filename, std::ios::binary | std::ios::ate);
        if (!fileStream.is_open())
        {
            throw std::runtime_error("Failed to open file for reading");
        }

        std::streampos currentSize = fileStream.tellg();
        if (currentSize > m_FilePosition)
        {
            fileStream.seekg(m_FilePosition);
            std::vector<unsigned char> buffer(static_cast<std::size_t>(currentSize - m_FilePosition));
            fileStream.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
            m_FilePosition = fileStream.tellg();

            m_Interpreter->interpret(buffer.data(), buffer.size());
        }

        fileStream.close();

        // Short sleep to prevent busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



