#include "Logger.h"


Logger* Logger::instance{nullptr};

Logger* Logger::getInstance() 
{
    if(instance == nullptr)
        instance = new Logger();

    return instance;
}

Logger::~Logger() 
{
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

void Logger::log(const unsigned char* packet, std::size_t length) 
{
    std::lock_guard<std::mutex> lock(mtx);
    if (fileStream.is_open()) {
        fileStream.write(reinterpret_cast<const char*>(packet), length);
    }
}

void Logger::setLogFile(const std::string& filename) 
{
    fileStream.open(filename, std::ios::binary | std::ios::app);
}