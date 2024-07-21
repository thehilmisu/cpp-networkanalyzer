#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <pcap.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

struct PcapFile 
{
    std::string srcIp;
    std::string dstIp;
    uint8_t protocol;
    std::size_t length;
    std::vector<unsigned char> data;
};

class PacketParser {
public:
    PacketParser();
    void setFilter(const std::string& srcIp, const std::string& dstIp);
    bool isMatchedFilter(const std::string& srcIp, const std::string& dstIp) const;
    PcapFile parse(const unsigned char* packet, std::size_t length);
    // Define the lookup table for IP protocol numbers
    std::unordered_map<int, std::string> ipProtocolNumbers;    
    std::string getProtocolName(int protocol_number);


private:
    std::string filterSrcIp;
    std::string filterDstIp;
};

#endif // PACKETPARSER_H
