#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <mutex>
#include <string>

class Logger 
{
public:
    static Logger* getInstance();
    void log(const unsigned char* packet, std::size_t length);
    void setLogFile(const std::string& filename);

private:
    Logger() = default;
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger *instance;
    std::ofstream fileStream;
    std::mutex mtx;
};

#endif // LOGGER_H
