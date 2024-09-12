#ifndef PACKETFILTERMANAGER_H
#define PACKETFILTERMANAGER_H

#include "IPacketFilter.h"
#include <vector>
#include <memory>

class PacketFilterManager 
{
public:
    void addFilter(std::shared_ptr<PacketFilter> filter);
    std::vector<PcapFile> applyFilters(const std::vector<PcapFile>& packets) const;
    void clearFilters();

private:
    bool matchesAllFilters(const PcapFile& packet) const;
    std::vector<std::shared_ptr<PacketFilter>> filters;  // Use std::shared_ptr instead of QSharedPointer
};

#endif // PACKETFILTERMANAGER_H
