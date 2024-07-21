#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <string>
#include <vector>
#include <pcap.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

class PacketParser {
public:
    struct ParsedPacket {
        std::string srcIp;
        std::string dstIp;
        uint8_t protocol;
        std::size_t length;
        std::vector<unsigned char> data;
    };

    PacketParser() : filterSrcIp(""), filterDstIp("") {}

    void setFilter(const std::string& srcIp, const std::string& dstIp) {
        filterSrcIp = srcIp;
        filterDstIp = dstIp;
    }

    bool matchesFilter(const std::string& srcIp, const std::string& dstIp) const {
        bool srcMatch = filterSrcIp.empty() || filterSrcIp == srcIp;
        bool dstMatch = filterDstIp.empty() || filterDstIp == dstIp;
        return srcMatch && dstMatch;
    }

    ParsedPacket parse(const unsigned char* packet, std::size_t length) {
        ParsedPacket parsedPacket;
        const struct ip* ipHeader = reinterpret_cast<const struct ip*>(packet);

        // Extract source and destination IP addresses
        parsedPacket.srcIp = inet_ntoa(ipHeader->ip_src);
        parsedPacket.dstIp = inet_ntoa(ipHeader->ip_dst);

        // Extract protocol
        parsedPacket.protocol = ipHeader->ip_p;

        // Extract total length
        parsedPacket.length = ntohs(ipHeader->ip_len);

        // Extract data (payload)
        const unsigned char* dataStart = packet + (ipHeader->ip_hl * 4);
        std::size_t dataLength = length - (ipHeader->ip_hl * 4);
        parsedPacket.data.assign(dataStart, dataStart + dataLength);

        return parsedPacket;
    }

private:
    std::string filterSrcIp;
    std::string filterDstIp;
};

#endif // PACKETPARSER_H
