#ifndef NETWORK_DEVICE_FINDER_H
#define NETWORK_DEVICE_FINDER_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "ConsoleHandler.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")
#elif defined(__APPLE__) || defined(__MACH__)
    #include <ifaddrs.h>
    #include <net/if_dl.h>
    #include <net/if.h>
    #include <arpa/inet.h>
#else
    #include <ifaddrs.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

class NetworkDeviceFinder 
{
public:
    NetworkDeviceFinder(const NetworkDeviceFinder&) = delete;
    NetworkDeviceFinder& operator=(const NetworkDeviceFinder&) = delete;
    static NetworkDeviceFinder& getInstance();
    std::string chooseDevice();

private:
    NetworkDeviceFinder(); 
    ~NetworkDeviceFinder();
    std::vector<std::string> listDevices(); 
    
};

#endif // NETWORK_DEVICE_FINDER_H
