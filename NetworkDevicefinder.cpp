#include "NetworkDeviceFinder.h"

#if defined(_WIN32) || defined(_WIN64)
    // Windows-specific implementation
    NetworkDeviceFinder::NetworkDeviceFinder() {}
    NetworkDeviceFinder::~NetworkDeviceFinder() {}

    NetworkDeviceFinder& NetworkDeviceFinder::getInstance() {
        static NetworkDeviceFinder instance;
        return instance;
    }

    std::vector<std::string> NetworkDeviceFinder::listDevices() {
        std::vector<std::string> devices;
        ULONG bufferSize = 15000;
        PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(bufferSize);

        if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &bufferSize) == ERROR_BUFFER_OVERFLOW) {
            free(pAddresses);
            pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(bufferSize);
        }

        if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &bufferSize) == NO_ERROR) {
            for (PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next) {
                devices.push_back(pCurrAddresses->AdapterName);
            }
        }

        if (pAddresses) {
            free(pAddresses);
        }

        return devices;
    }

#elif defined(__APPLE__) || defined(__MACH__)
    // macOS-specific implementation
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

#else
    // UNIX-like (Linux) specific implementation
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

    std::cout << "Available network devices:\n";
    for (size_t i = 0; i < devices.size(); ++i) {
        std::cout << i + 1 << ". " << devices[i] << "\n";
    }

    std::cout << "Choose a device (1-" << devices.size() << "): ";
    size_t choice;
    std::cin >> choice;

    if (choice < 1 || choice > devices.size()) {
        return "";
    }

    return devices[choice - 1];
}
