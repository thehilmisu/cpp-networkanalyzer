#ifndef PCAPFILE_H
#define PCAPFILE_H

#include <vector>
#include <string>
#include <cstdint>

struct PacketLineData
{
    std::string offset;
    std::string hexPart;
    std::string asciiPart;
};


struct PcapFile
{
    std::string srcIp;
    std::string dstIp;
    uint8_t protocol_number;
    std::string protocol_name;
    std::size_t length;
    std::vector<unsigned char> data;
    std::string formattedData;
    std::string detectedLinks;
    std::vector<PacketLineData> packetLineData;
};

#endif // PCAPFILE_H
