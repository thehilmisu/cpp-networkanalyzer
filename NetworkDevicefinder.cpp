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
    NetworkDeviceFinder::NetworkDeviceFinder() {}
    NetworkDeviceFinder::~NetworkDeviceFinder() {}

    NetworkDeviceFinder& NetworkDeviceFinder::getInstance() 
    {
        static NetworkDeviceFinder instance;
        return instance;
    }

    std::vector<std::string> NetworkDeviceFinder::listDevices() 
    {
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

    NetworkDeviceFinder& NetworkDeviceFinder::getInstance() 
    {
        static NetworkDeviceFinder instance;
        return instance;
    }

    std::vector<std::string> NetworkDeviceFinder::listDevices() 
    {
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
    if (devices.empty()) 
    {
        std::cerr << "No network devices found." << std::endl;
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < devices.size(); ++i) 
    {
        ConsoleHandler::getInstance().print("[" + std::to_string(i + 1) + "] " + devices[i] );
    }

    size_t choice = std::stoi(ConsoleHandler::getInstance().input("Enter the index of the device you want to use: "));

    while (choice < 1 || choice > devices.size()) 
    {
        choice = std::stoi(ConsoleHandler::getInstance().input("Invalid selection! Please enter a number between 1 and " + std::to_string(devices.size()) + " \n"));
    }

    return devices[choice - 1];
}
