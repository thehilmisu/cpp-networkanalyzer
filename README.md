# cpp-networkanalyzer
simple tool to analyze network packages (pcap files)

Class Diagram
-------------
                      +-----------------------------+
                      |          Monitor            |
                      +-----------------------------+
                      | + monitor() : void          |
                      +-----------------------------+
                                            ^
                                            |
                      +-----------------------------+
                      |         FileMonitor         |
                      +-----------------------------+
                      | - filename: string          |
                      | - filePosition: streampos   |
                      | - interpreter:              |
                      |     PcapInterpreter         |
                      +-----------------------------+
                      | + FileMonitor(              |
                      |     filename: string)       |
                      | + setPacketFilter(          |
                      |     srcIp: string,          |    
                      | dstIp: string) : void       |
                      | + monitor() : void          |
                      | - print(buffer:             |
                      |     const unsigned char*,   |
                      |     length: size_t) : void  |
                      +-----------------------------+
                                            |
                      +-----------------------------+
                      |       PcapInterpreter       |
                      +-----------------------------+
                      | - filterSrcIp: string       |
                      | - filterDstIp: string       |
                      | - ipProtocolNumbers:        |
                      |  unordered_map<int, string> |
                      +-----------------------------+
                      | + PcapInterpreter()         |
                      | + setFilter(srcIp: string,  |
                      |     dstIp: string) : void   |
                      | + isMatchedFilter(          |
                      |   srcIp: string,            |
                      |   dstIp: string) : bool     |
                      | + interpret(                |
                      |     packet:                 |
                      |        const unsigned char*,|
                      |        length: size_t) :    |
                      |             PcapFile        |
                      | + getProtocolName(          |
                      |     protocol_number: int) : |
                      |           string            |
                      +-----------------------------+
                                            |
                      +-----------------------------+
                      |            Logger           |
                      +-----------------------------+
                      | - instance: Logger*         |
                      | - fileStream: ofstream      |
                      | - mtx: mutex                |
                      +-----------------------------+
                      | + getInstance() : Logger*   |
                      | + log(packet:               |
                      |     const unsigned char*,   |
                      |     length: size_t) : void  |
                      | + setLogFile(               |
                      |     filename: string) : void|
                      | - Logger() = default        |
                      | - ~Logger()                 |
                      | - Logger(                   |
                      |  const Logger&) = delete    |
                      | - Logger&                   |
                      |     operator=(              |    
                      |     const Logger&) = delete |
                      +-----------------------------+

