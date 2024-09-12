#include "packetfiltermanager.h"

void PacketFilterManager::addFilter(std::shared_ptr<PacketFilter> filter)
{
    filters.push_back(filter);
}

std::vector<PcapFile> PacketFilterManager::applyFilters(const std::vector<PcapFile>& packets) const
{
    std::vector<PcapFile> filteredPackets;
    for (const auto& packet : packets)
    {
        if (matchesAllFilters(packet))
        {
            filteredPackets.push_back(packet);
        }
    }
    return filteredPackets;
}

void PacketFilterManager::clearFilters()
{
    filters.clear();
}

bool PacketFilterManager::matchesAllFilters(const PcapFile& packet) const
{
    for (const auto& filter : filters)
    {
        if (!filter->matches(packet))
        {
            return false;
        }
    }
    return true;
}