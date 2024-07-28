# cpp-networkanalyzer
simple tool to analyze network packages (pcap files)

## class diagram.

![Screenshot from 2024-07-28 17-13-55](https://github.com/user-attachments/assets/30e0fe91-ad9d-4f48-93ae-8862a2e17dcb)


## what can be done to improve?
 - Implement dependency injection on FileMonitor to have the PcapInterpreter as a dependency 
   to have the flexibility of other file types
 
 - FileMonitor's sleep structure can be improved?

 - All classes can be modified according to single repsonsibility principle
    - FileMonitor now monitoring the file and printing it on the console. instead, It can be seperated

 - Dependency inverison can be achieved??
    - Instead of having filemonitor object directly in the main, we can have interface object for that
    


