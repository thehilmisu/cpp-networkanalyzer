#include "FileMonitor.h"

void FileMonitor::setPacketFilter(const std::string& srcIp, const std::string& dstIp)
{
    parser.setFilter(srcIp, dstIp);
}

void FileMonitor::monitor(const unsigned char* packet, std::size_t length)  
{
        // This method is not used in this version since processing is done in `process()`
}

void FileMonitor::process()
{
    while (true) 
    {
        std::ifstream fileStream(filename, std::ios::binary | std::ios::ate);
        if (!fileStream.is_open()) 
        {
            throw std::runtime_error("Failed to open file for reading");
        }

        std::streampos currentSize = fileStream.tellg();
        if (currentSize > filePosition) 
        {
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

void FileMonitor::processBuffer(const unsigned char* buffer, std::size_t length) 
{
        PcapFile parsedPacket = parser.parse(buffer, length);
        
        bool isMatch = parser.isMatchedFilter(parsedPacket.srcIp, parsedPacket.dstIp);

        std::ostringstream oss;
        oss << (isMatch ? "[MATCH] " : "[NO MATCH] ")
            << "<Source IP: " << parsedPacket.srcIp << "> "
            << "<Destination IP: " << parsedPacket.dstIp << "> "
            << "<Protocol: " << parser.getProtocolName(static_cast<int>(parsedPacket.protocol)) << "> "
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
