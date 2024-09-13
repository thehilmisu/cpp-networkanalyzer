#ifndef PCAPINTERPRETER_H
#define PCAPINTERPRETER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <pcap.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "IInterpreter.h"
#include "pcapfile.h"

class PcapInterpreter : public IInterpreter<PcapFile>
{

public:
    explicit PcapInterpreter();
    virtual ~PcapInterpreter() = default;
    void setFilter(const std::string& srcIp, const std::string& dstIp);
    bool isMatchedFilter(const std::string& srcIp, const std::string& dstIp) const;
    PcapFile interpret(const unsigned char* packet, std::size_t length) override;
    std::vector<PacketLineData> getPacketLineData(const std::vector<unsigned char>& data);

private:
    std::string m_FilterSrcIp;
    std::string m_FilterDstIp;
    std::string formatPacketData(const std::vector<unsigned char>& data);
    std::string formatPacketDataContinuation(const std::vector<unsigned char>& data);
    bool isWordContinuation(const std::string& prevLine, const std::string& currentLine);
    std::string detectLinksAndAPICalls(const std::vector<unsigned char>& data);


};

#endif // PCAPINTERPRETER_H
