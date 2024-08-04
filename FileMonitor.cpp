#include "FileMonitor.h"


void FileMonitor::monitor()  
{
    while (true) 
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

            m_Interpreter.interpret(buffer.data(), buffer.size());
        }

        fileStream.close();

        // Short sleep to prevent busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}