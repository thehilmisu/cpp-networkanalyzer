# cpp-networkanalyzer
simple tool to analyze network packages (pcap files)

Platform independent, lists your network devices to analyze network packages
Tested on ubuntu and mac os. Should work on windows but did not test on that. 

### Compile

     make all

### Run
    sudo permission is needed to be able to read the network interface

    sudo ./build/network_analyzer

# Class diagram

```mermaid
classDiagram

    class FileMonitor {
        - m_Filename : std::string
        - m_FilePosition : std::streampos
        - m_Interpreter : PcapInterpreter
        + FileMonitor(filename: std::string)
        + monitor()  void override
    }

    class Monitor {
        <<interface>>
        + monitor()  void
    }

    class Interpreter {
        <<interface>>
        + interpret(const unsigned char* packet, std::size_t length)  T
    }
    
    class PcapFile {
        + srcIp : std::string
        + dstIp : std::string 
        + protocol_number : uint8_t
        + protocol_name : std::string
        + length : std::size_t
        + data : std::vector<unsigned char>
    }

    class PcapInterpreter {
        +PcapInterpreter()
        +setFilter(const std::string& srcIp, const std::string& dstIp) void
        +isMatchedFilter(const std::string& srcIp, const std::string& dstIp)  const bool
        +interpret(const unsigned char* packet, std::size_t length)  void
        + getProtocolName(int protocol_number) std::string
        - m_FilterSrcIp : std::string
        - m_FilterDstIp : std::string
        - ipProtocolNumbers : std::unordered_map<int, std::string> 
        - printPackets(PcapFile parsedFile, bool isMatch) void
    }
    
    class ConsoleHandler {
        <<singleton>>
        + getInstance()  ConsoleHandler&
        + print(message: std::string) void
        + input(prompt: std::string) std::string
    }

    
    class Logger {
        <<singleton>>
        - m_FileStream : std:ofstream
        - m_Mutex : std::mutex
        + getInstance()  Logger&
        + log(packet: const unsigned char*, length: std::size_t) void
        + setLogFile(filename: std::string) void
    }

    class NetworkDeviceFinder {
        <<singleton>>
        + getInstance()  NetworkDeviceFinder&
        + chooseDevice()  std::string
        - listDevices()  std::vector<std::string>
    }

    %% Inheritance
    FileMonitor ..|> Monitor : <<inherits>>
    PcapInterpreter ..|> Interpreter : <<inherits>>

    %% Usage relationships
    FileMonitor ..> PcapInterpreter : uses
    PcapInterpreter ..> PcapFile : uses
    PcapInterpreter ..> ConsoleHandler: uses
    NetworkDeviceFinder ..> ConsoleHandler: uses

    ConsoleHandler ..> ConsoleHandler : <<getInstance()>>
    Logger ..> Logger : <<getInstance()>>
    NetworkDeviceFinder ..> NetworkDeviceFinder : <<getInstance()>>



```
