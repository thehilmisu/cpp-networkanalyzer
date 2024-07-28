#include "Logger.h"
#include "FileMonitor.h"
#include <pcap.h>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <iostream>
#include <ifaddrs.h>
#include <netdb.h>
#include <vector>

std::string chooseNetworkDevice() 
{
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    std::vector<std::string> devices;
    
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    std::cout << "Available network interfaces:" << std::endl;
    int index = 1;
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {
            s = getnameinfo(ifa->ifa_addr,
                            (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                  sizeof(struct sockaddr_in6),
                            host, NI_MAXHOST,
                            NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                std::cout << "getnameinfo() failed: " << gai_strerror(s) << std::endl;
                continue;
            }

            std::string deviceName = ifa->ifa_name;
            std::cout << index << ": " << deviceName << " (" << host << ")" << std::endl;
            devices.push_back(deviceName);
            index++;
        }
    }
    freeifaddrs(ifaddr);

    int choice;
    std::cout << "Enter the number of the device you want to use: ";
    std::cin >> choice;
    std::cout << "------------------------------------------------" << std::endl;

    // Validate the input
    while (std::cin.fail() || choice < 1 || choice > devices.size()) {
        std::cin.clear(); // clear error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore last input
        std::cout << "Invalid selection. Please enter a number between 1 and " << devices.size() << ": ";
        std::cin >> choice;
    }

    return devices[choice - 1]; // return the selected device name
}


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

    std::string dev = chooseNetworkDevice();
    const std::string filename = "packets.pcap";

    // Set up logging
    Logger::getInstance()->setLogFile(filename);

    // Set up monitoring
    FileMonitor fileMonitor(filename);
    fileMonitor.setPacketFilter("192.168.1.1", "192.168.1.2");

    // Start threads
    std::thread captureThreadObj([&] { captureThread(dev.c_str()); });
    std::thread monitoringThreadObj([&] { fileMonitor.monitor(); });

    // Wait for both threads to complete
    captureThreadObj.join();
    monitoringThreadObj.join();

}
