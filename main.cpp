#include "logger.h"
#include "filemonitor.h"
#include "networkdevicefinder.h"
#include "pcapinterpreter.h"
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
#include "ui/Textbox.h"
#include "ThreadSafeQueue.h"


std::atomic<bool> exitFlag(false);
ThreadSafeQueue<PcapFile> packetQueue;
auto interpreter = std::make_unique<PcapInterpreter>();

void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet) 
{
    packetQueue.push(interpreter.get()->interpret(packet,sizeof(packet)));
    // Log the packet as before
    Logger::getInstance().log(packet, pkthdr->len);
}

void captureThread(std::string dev) 
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live(dev.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        std::cerr << "Error opening pcap handle: " << errbuf << std::endl;
        return;
    }

    while (!exitFlag) {
        pcap_dispatch(handle, 0, packetHandler, nullptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    pcap_close(handle);
}


int main() 
{
    std::vector<std::string> devList = NetworkDeviceFinder::getInstance().listDevices();
    const std::string filename = "packets.pcap";
    Logger::getInstance().setLogFile(filename);

    // Initialize TUIManager, todo: find another way to do it
    TUIManager tuiManager;
    tuiManager.initWindow();

    auto packetTable = std::make_shared<TableWidget>(Position{0, 8}, 4, 10); // 4 columns
    packetTable->setHeaders({"Source IP", "Destination IP", "Protocol", "Length"});

    tuiManager.placeElement(packetTable);

    auto dropDownLabel = std::make_shared<Label>("Select network device: ",GREEN_ON_BLACK,Position{0, 0});
    auto deviceDropdown = std::make_shared<DropdownWidget>(Position{25, 0}, devList);

    // Shared variables for threads
    std::thread captureThreadObj;
    //std::thread monitoringThreadObj;
    bool threadsRunning = false;

    // Connect the onOptionSelected function to the dropdown's onOptionSelected signal
    deviceDropdown->onOptionsSelected.connect([&](const std::string& option) {
       
        // Start threads if not already running
        if (!threadsRunning) {
            threadsRunning = true;
            captureThreadObj = std::thread(captureThread, option);
            //monitoringThreadObj = std::thread(&FileMonitor::run, &fileMonitor);
        } else {
            // Implement logic to handle restarting threads if necessary
            // For example, stop existing threads and start new ones
        }
        refresh();
    });

    tuiManager.placeElement(dropDownLabel);
    tuiManager.placeElement(deviceDropdown);


    //filter ui group
    auto filterLabel = std::make_shared<Label>("Filter: ",GREEN_ON_BLACK,Position{0, 5}); 
    auto filterTextBox = std::make_shared<TextBox>(Position{10, 5}, 30); // Position at y=2, x=0, width=30
    auto applyFilterButton = std::make_shared<Button>("Apply Filter",[]() {  },YELLOW_ON_BLACK,Position{42,5});
    tuiManager.placeElement(filterTextBox);
    tuiManager.placeElement(filterLabel);
    tuiManager.placeElement(applyFilterButton);


    // Main UI loop
    while (!exitFlag) {
        // Update UI with new packets if necessary
        PcapFile file;
        while (packetQueue.tryPop(file)) {
            packetTable->addRow({file.srcIp, file.dstIp, file.protocol_name, std::to_string(file.length)});
        }
               
        // Draw UI
        tuiManager.drawUI();

        // Handle input
        tuiManager.handleInput(exitFlag);

        // Sleep to prevent high CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Check for exit condition (e.g., user presses 'q')
        // if (userWantsToExit) {
        //     exitFlag = true;
        // }
    }

    // Signal threads to exit
    if (threadsRunning) {
        // Implement a method to stop the threads gracefully
        // For example:
        //fileMonitor.stop(); // You need to implement this method
        captureThreadObj.join();
        //monitoringThreadObj.join();
    }

    tuiManager.endWindow();

    return 0;
}

