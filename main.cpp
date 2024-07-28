#include "Logger.h"
#include "FileMonitor.h"
#include <pcap.h>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <iostream>


// Packet handler function for libpcap
void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet) 
{
    Logger::getInstance()->log(packet, pkthdr->len);
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
    const char* dev = "wlp4s0"; // Replace with your network device name
    const std::string filename = "packets.pcap";

    // Set up logging
    Logger::getInstance()->setLogFile(filename);

    // Set up monitoring
    FileMonitor fileMonitor(filename);

    // Set filters for packets
    fileMonitor.setPacketFilter("192.168.1.1", "192.168.1.2"); // Replace with desired IP addresses

    // Start the capture thread
    std::thread captureThreadObj(captureThread, dev);
    // Start the monitoring thread
    std::thread monitoringThreadObj([&] { fileMonitor.monitor(); });

    // Wait for both threads to complete
    captureThreadObj.join();
    monitoringThreadObj.join();

    return 0;
}
