#include "FileMonitor.h"

void FileMonitor::setPacketFilter(const std::string& srcIp, const std::string& dstIp)
{
    interpreter.setFilter(srcIp, dstIp);
}

void FileMonitor::monitor()  
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

            PcapFile parsedPacket = interpreter.interpret(buffer.data(), buffer.size());
        
            bool isMatch = interpreter.isMatchedFilter(parsedPacket.srcIp, parsedPacket.dstIp);

            ConsoleHandler::getInstance().printPackets(interpreter, parsedPacket, isMatch);
        }

        fileStream.close();

        // Short sleep to prevent busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}