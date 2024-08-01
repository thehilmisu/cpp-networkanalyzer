#include "NetworkDeviceFinder.h"

NetworkDeviceFinder::NetworkDeviceFinder() 
{
    #if defined(_WIN32) || defined(_WIN64)
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif
}

NetworkDeviceFinder::~NetworkDeviceFinder() 
{
    #if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
    #endif
}

NetworkDeviceFinder& NetworkDeviceFinder::getInstance() 
{
    static NetworkDeviceFinder instance;
    return instance;
}

std::string NetworkDeviceFinder::chooseDevice() 
{
    std::vector<std::string> devices = listDevices();
    if (devices.empty()) {
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

std::vector<std::string> NetworkDeviceFinder::listDevices() 
{
    std::vector<std::string> devices;
    #if defined(_WIN32) || defined(_WIN64)
        ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
        ULONG family = AF_UNSPEC;
        PIP_ADAPTER_ADDRESSES addresses = nullptr, outAddresses = nullptr;
        ULONG outBufLen = 15000; // Set an initial buffer size.
        DWORD dwRetVal = 0;

        do 
        {
            addresses = (IP_ADAPTER_ADDRESSES*) new char[outBufLen];
            dwRetVal = GetAdaptersAddresses(family, flags, nullptr, addresses, &outBufLen);
            if (dwRetVal == ERROR_BUFFER_OVERFLOW) 
            {
                delete[] addresses;
                addresses = nullptr;
            } 
            else 
            {
                break;
            }
        } while ((dwRetVal == ERROR_BUFFER_OVERFLOW));

        if (dwRetVal == NO_ERROR) 
        {
            for (outAddresses = addresses; outAddresses != nullptr; outAddresses = outAddresses->Next) {
                devices.push_back(std::string(outAddresses->AdapterName));
            }
        }
        if (addresses) 
        {
            delete[] addresses;
        }
    #else
        struct ifaddrs *ifaddr, *ifa;

        if (getifaddrs(&ifaddr) == -1) 
        {
            perror("getifaddrs");
            exit(EXIT_FAILURE);
        }

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
        {
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET) 
            {
                devices.push_back(ifa->ifa_name);
            }
        }

        freeifaddrs(ifaddr);
        
    #endif

    return devices;
}