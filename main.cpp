#include "Logger.h"
#include "FileMonitor.h"
#include <pcap.h>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "NetworkDeviceFinder.h"

// Packet handler function for libpcap
void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet) 
{
    Logger::getInstance().log(packet, pkthdr->len);
}

// Packet capturing thread function
void captureThread(const char* dev) 
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        std::cerr << "Error opening pcap handle: " << errbuf << std::endl;
        return;
    }

    pcap_loop(handle, 0, packetHandler, nullptr);
    pcap_close(handle);
}

int main() 
{
    std::string dev = NetworkDeviceFinder::getInstance().chooseDevice();

    const std::string filename = "packets.pcap";
    Logger::getInstance().setLogFile(filename);

    FileMonitor fileMonitor(filename);
    //fileMonitor.setPacketFilter("192.168.1.1", "192.168.1.2");

    std::thread captureThreadObj([&] { captureThread(dev.c_str()); });
    std::thread monitoringThreadObj([&] { fileMonitor.monitor(); });

    captureThreadObj.join();
    monitoringThreadObj.join();

}
