#include <iostream>
#include <vector>
#include <string>
#include <limits>

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")
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
