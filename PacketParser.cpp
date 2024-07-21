#include "PacketParser.h"

PacketParser::PacketParser() : filterSrcIp(""), filterDstIp("") 
{
    ipProtocolNumbers = {
        {0, "HOPOPT"},       // IPv6 Hop-by-Hop Option
        {1, "ICMP"},         // Internet Control Message Protocol
        {2, "IGMP"},         // Internet Group Management Protocol
        {3, "GGP"},          // Gateway-to-Gateway Protocol
        {4, "IP-in-IP"},     // IP in IP (encapsulation)
        {5, "ST"},           // Stream Protocol
        {6, "TCP"},          // Transmission Control Protocol
        {8, "EGP"},          // Exterior Gateway Protocol
        {9, "IGP"},          // Interior Gateway Protocol
        {17, "UDP"},         // User Datagram Protocol
        {27, "RDP"},         // Reliable Datagram Protocol
        {41, "IPv6"},        // IPv6 encapsulation
        {43, "IPv6-Route"},  // Routing Header for IPv6
        {44, "IPv6-Frag"},   // Fragment Header for IPv6
        {47, "GRE"},         // Generic Routing Encapsulation
        {50, "ESP"},         // Encap Security Payload
        {51, "AH"},          // Authentication Header
        {58, "ICMPv6"},      // ICMP for IPv6
        {59, "IPv6-NoNxt"},  // No Next Header for IPv6
        {60, "IPv6-Opts"},   // Destination Options for IPv6
        {88, "EIGRP"},       // EIGRP
        {89, "OSPF"},        // Open Shortest Path First
        {94, "IPIP"},        // IP-within-IP Encapsulation Protocol
        {97, "ETHERIP"},     // Ethernet-within-IP Encapsulation
        {112, "VRRP"},       // Virtual Router Redundancy Protocol
        {115, "L2TP"},       // Layer Two Tunneling Protocol
        {132, "SCTP"},       // Stream Control Transmission Protocol
        {136, "UDPLite"},    // Lightweight User Datagram Protocol
        {137, "MPLS-in-IP"}  // MPLS-in-IP
    };
}


void PacketParser::setFilter(const std::string& srcIp, const std::string& dstIp) 
{
    filterSrcIp = srcIp;
    filterDstIp = dstIp;
}

bool PacketParser::isMatchedFilter(const std::string& srcIp, const std::string& dstIp) const 
{
    bool srcMatch = filterSrcIp.empty() || filterSrcIp == srcIp;
    bool dstMatch = filterDstIp.empty() || filterDstIp == dstIp;
    return srcMatch && dstMatch;
}

std::string PacketParser::getProtocolName(int protocol_number) 
{
    std::cout << protocol_number << std::endl;
    auto it = ipProtocolNumbers.find(protocol_number);
    if (it != ipProtocolNumbers.end()) 
        return it->second;
     
    return "Unknown Protocol";
}
    
PcapFile PacketParser::parse(const unsigned char* packet, std::size_t length) 
{
    PcapFile parsedPacket;
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