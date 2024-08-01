# cpp-networkanalyzer
simple tool to analyze network packages (pcap files)

Platform independent, lists your network devices to analyze network packages
Tested on ubuntu and mac os. Should work on windows but did not test on that. 

To compile
    ```
        make all

    ``` 
To clean
    ```
     make clean 
    ``` 

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

 - ConsoleHandler can be seperated to have a interface??



