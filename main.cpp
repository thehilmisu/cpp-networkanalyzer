#include "logger.h"
#include "filemonitor.h"
#include "networkdevicefinder.h"
#include <pcap.h>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <iostream>

#include "ui/TUIManager.h"
#include "ui/Label.h"
#include "ui/Button.h"
#include "ui/TableWidget.h"
#include "ui/DropdownWidget.h"


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

    auto interpreter = std::make_unique<PcapInterpreter>();

    FileMonitor &fileMonitor = FileMonitor::getInstance();
    fileMonitor.setPcapInterpreter(interpreter.get());
    fileMonitor.setFileName(filename);

    std::thread captureThreadObj([&] { captureThread(dev.c_str()); });
    std::thread monitoringThreadObj([&] { fileMonitor.run(); });

    captureThreadObj.join();
    monitoringThreadObj.join();

}
