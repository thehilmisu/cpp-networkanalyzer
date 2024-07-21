#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <mutex>
#include <string>

class Logger 
{
public:
    static Logger& getInstance() 
    {
        static Logger instance;
        return instance;
    }

    void log(const unsigned char* packet, std::size_t length) 
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (fileStream.is_open()) {
            fileStream.write(reinterpret_cast<const char*>(packet), length);
        }
    }

    void setLogFile(const std::string& filename) 
    {
        fileStream.open(filename, std::ios::binary | std::ios::app);
    }

private:
    Logger() = default;
    ~Logger() 
    {
        if (fileStream.is_open()) {
            fileStream.close();
        }
    }

    std::ofstream fileStream;
    std::mutex mtx;

    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_H
