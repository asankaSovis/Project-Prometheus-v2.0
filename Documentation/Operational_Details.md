This section will introduce the basic operational details of the Prometheus Fire Alarm System v2.0. It will cover topics of [Operational Architecture](# Operational Architecture), [Features and Advantages of the System](# Features and Advantages of the System), [Technical Limitations](# Technical Limitations) and [Communication Architecture](# Communication Architecture).
For comprehensive introduction about the hardware architecture, refer to the Hardware Architecture section. Likewise, refer to the Software Architecture of a comprehensive introduction to the Software Architecture.
# Operational Architecture
This fire alarm system is designed to detect, indicate and extinguish fire by using necessary precautions. The system is expected to consist of three main stages.
1. Fire sensing and extinguishing system
2. Control stage
3. Alarming and notification stage
There are four main sections and components for the proper functioning of the system. It will help to complete that all the stages that mentioned above.
1. Fire sensors
2. Extinguishing valves
3. Control unit
4. Software system
5. Structural design
The system is designed to support as many of the Fire Alarm Modules as required according to the implemented environment. For example, if we are implementing in a school, each classroom can have individual modules. But accordingly, we will have to upgrade the control system used.

![[Design.drawio.png]]

>  Operational Structure
# Features and Advantages of the System
From a design standpoint, Prometheus Fire Alarm System v1.0 had a unique design philosophy that allowed it to be capable of evolving it into an IoT based fire alarm system. This is because a key importance was given to make it not just a monitoring and extinguishing device, but to go even further and become a user interactable and customizable system.
This is what was capitalized to integrate the Prometheus Fire Alarm System with the Matter protocol. For a comprehensive discussion on Matter and Prometheus Fire Alarm System, refer to [Limitations and Future](./Limitations_and_Future) section. Some features and advantages will be discussed below.
- Real-time monitoring of the environment
- Real-time detection and reaction system
- Complete operation done within the system itself without offloading (Safer and more reliable operation)
- Real-time stats can be accessed via the control panel/matter home network
- Flexibility to integrate newer communication systems to increase functionality
- Fully open-sourced hardware and software architecture allowing transparency and learning opportunities
# Technical Limitations
Above advantages does come with some limitations. Everything was done by myself alone in my spare time juggling both work, studies and my personal life. With the limited time and budget available, I am proud of what I've achieved. However, it is important to acknowledge the limitations that currently exist in the developed system.
- Incomplete Matter integration
- No method to handle power loss
- No checks in place to indicate if water line goes dry
- Existing communication system, though robust, is limited and inefficient
- nRF kit is not integrated into a single PCB solution
- Slight issues with the PCB layout of the Prometheus PCB
