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

    void setPacketFilter(const std::string& srcIp, const std::string& dstIp) {
        parser.setFilter(srcIp, dstIp);
    }

    void monitor(const unsigned char* packet, std::size_t length) override {
        // This method is not used in this version since processing is done in `process()`
    }

    void process() {
        while (true) {
            std::ifstream fileStream(filename, std::ios::binary | std::ios::ate);
            if (!fileStream.is_open()) {
                throw std::runtime_error("Failed to open file for reading");
            }

            std::streampos currentSize = fileStream.tellg();
            if (currentSize > filePosition) {
                fileStream.seekg(filePosition);
                std::vector<unsigned char> buffer(static_cast<std::size_t>(currentSize - filePosition));
                fileStream.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
                filePosition = fileStream.tellg();
                processBuffer(buffer.data(), buffer.size());
            }

            fileStream.close();

            // Short sleep to prevent busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

private:
    void processBuffer(const unsigned char* buffer, std::size_t length) {
        PacketParser::ParsedPacket parsedPacket = parser.parse(buffer, length);
        bool isMatch = parser.matchesFilter(parsedPacket.srcIp, parsedPacket.dstIp);

        std::ostringstream oss;
        oss << (isMatch ? "[MATCH] " : "[NO MATCH] ")
            << "<Source IP: " << parsedPacket.srcIp << "> "
            << "<Destination IP: " << parsedPacket.dstIp << "> "
            << "<Protocol: " << static_cast<int>(parsedPacket.protocol) << "> "
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

    std::string filename;
    std::streampos filePosition;
    PacketParser parser;
};

#endif // FILEMONITOR_H
