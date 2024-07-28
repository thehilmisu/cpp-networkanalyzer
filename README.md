# cpp-networkanalyzer
simple tool to analyze network packages (pcap files)

## class diagram

+---------------------------------------------------------------+
|                           <<abstract>>                        |
|                             Monitor                           |
+---------------------------------------------------------------+
| + monitor(): void                                             |
+---------------------------------------------------------------+
             ^
             | (inherits)
             |
+---------------------------------------------------------------+
|                           FileMonitor                         |
+---------------------------------------------------------------+
| - filename: string                                            |
| - filePosition: streampos                                     |
| - interpreter: PcapInterpreter                                |
+---------------------------------------------------------------+
| + FileMonitor(filename: string)                               |
| + setPacketFilter(srcIp: string, dstIp: string): void         |
| + monitor(): void                                             |
| - print(buffer: const unsigned char*, length: size_t): void   |
+---------------------------------------------------------------+
      | 1 (association)
      | uses
      V
+---------------------------------------------------------------+           +---------------------------------------------------------------+
|                         PcapInterpreter                       |           |                               Logger                          |
+---------------------------------------------------------------+           +---------------------------------------------------------------+
| - filterSrcIp: string                                         |           | - instance: Logger*                                           |
| - filterDstIp: string                                         |           | - fileStream: ofstream                                        |
| + ipProtocolNumbers:                                          |           | - mtx: mutex                                                  |
|   unordered_map<int, string>                                  |           +---------------------------------------------------------------+
+---------------------------------------------------------------+           | + getInstance(): Logger*                                      |
| + PcapInterpreter()                                           |           | + log(packet: const unsigned char*, length: size_t): void     |
| + setFilter(srcIp: string, dstIp: string): void               |           | + setLogFile(filename: string): void                          |
| + isMatchedFilter(srcIp: string, dstIp: string): bool         |           | - Logger(): default                                           |
| + interpret(packet: const unsigned char*, length: size_t):    |           | - ~Logger()                                                   |
|   PcapFile                                                    |           | - Logger(const Logger&) = delete                              |
| + getProtocolName(protocol_number: int): string               |           | - Logger& operator=(const Logger&) = delete                   |
+---------------------------------------------------------------+           +---------------------------------------------------------------+
