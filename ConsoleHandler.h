#ifndef CONSOLE_MANAGER_H
#define CONSOLE_MANAGER_H

#include <iostream>
#include <iostream>
#include <sstream>
#include <string>
#include "PcapInterpreter.h"


class ConsoleHandler
{
public:
    ConsoleHandler(const ConsoleHandler&) = delete;
    ConsoleHandler& operator=(const ConsoleHandler&) = delete;
    
    static ConsoleHandler& getInstance()
    {
        static ConsoleHandler instance;

        return instance;

    }
    static void print(const std::string& message) 
    {
        std::cout << message << std::endl;
    }
    static void printPackets(PcapInterpreter interpreter, PcapFile parsedPacket, bool isMatch)
    {
        std::ostringstream oss;
        oss << (isMatch ? "[MATCH] " : "[NO MATCH] ")
            << "<Source IP: " << parsedPacket.srcIp << "> "
            << "<Destination IP: " << parsedPacket.dstIp << "> "
            << "<Protocol: " << interpreter.getProtocolName(static_cast<int>(parsedPacket.protocol)) << "> "
            << "<Length: " << parsedPacket.length << "> " ;
        //     << "<Data: ";

        // for (const auto& byte : parsedPacket.data) {
        //     if (std::isprint(byte)) {
        //         oss << byte;
        //     } else {
        //         oss << '.';
        //     }
        // }
        oss << ">";

        std::cout << oss.str() << std::endl;
    }
    static std::string input(const std::string& prompt) 
    {
        std::cout << prompt;
        std::string response;
        std::getline(std::cin, response);
        return response;
    }

private:
    ConsoleHandler() {}
    static ConsoleHandler *instance;
};

#endif // CONSOLE_MANAGER_H
