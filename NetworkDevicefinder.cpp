#include "NetworkDeviceFinder.h"

#if defined(_WIN32) || defined(_WIN64)
    // Windows-specific includes and code
    NetworkDeviceFinder::NetworkDeviceFinder() {}
    NetworkDeviceFinder::~NetworkDeviceFinder() {}

    NetworkDeviceFinder& NetworkDeviceFinder::getInstance() {
        static NetworkDeviceFinder instance;
        return instance;
    }

    std::vector<std::string> NetworkDeviceFinder::listDevices() {
        std::vector<std::string> devices;
        // Windows-specific code to list network devices
        // Use GetAdaptersAddresses() or similar API
        return devices;
    }

#elif defined(__APPLE__) || defined(__MACH__)
    // macOS-specific includes and code
    #ifndef NI_MAXHOST
    #define NI_MAXHOST 1025
    #endif

    #ifndef NI_NUMERICHOST
    #define NI_NUMERICHOST 1
    #endif

    NetworkDeviceFinder::NetworkDeviceFinder() {}
    NetworkDeviceFinder::~NetworkDeviceFinder() {}

    NetworkDeviceFinder& NetworkDeviceFinder::getInstance() {
        static NetworkDeviceFinder instance;
        return instance;
    }

    std::vector<std::string> NetworkDeviceFinder::listDevices() {
        std::vector<std::string> devices;
        struct ifaddrs *ifaddr, *ifa;
        if (getifaddrs(&ifaddr) == -1) {
            perror("getifaddrs");
            return devices;
        }

        for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr) continue;

            if (ifa->ifa_addr->sa_family == AF_INET) { // Check for IPv4
                char host[NI_MAXHOST];
                if (getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST) == 0) {
                    devices.push_back(ifa->ifa_name);
                }
            }
        }

        freeifaddrs(ifaddr);
        return devices;
    }

#else
    // UNIX-like (Linux) specific code
    NetworkDeviceFinder::NetworkDeviceFinder() {}
    NetworkDeviceFinder::~NetworkDeviceFinder() {}

    NetworkDeviceFinder& NetworkDeviceFinder::getInstance() {
        static NetworkDeviceFinder instance;
        return instance;
    }

    std::vector<std::string> NetworkDeviceFinder::listDevices() {
        std::vector<std::string> devices;
        struct ifaddrs *ifaddr, *ifa;
        if (getifaddrs(&ifaddr) == -1) {
            perror("getifaddrs");
            return devices;
        }

        for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr) continue;

            if (ifa->ifa_addr->sa_family == AF_INET) { // Check for IPv4
                devices.push_back(ifa->ifa_name);
            }
        }

        freeifaddrs(ifaddr);
        return devices;
    }

#endif

std::string NetworkDeviceFinder::chooseDevice() {
    std::vector<std::string> devices = listDevices();
    if (devices.empty()) {
        return "";
    }

    // Assuming the first device is selected for simplicity
    return devices[0];
}
