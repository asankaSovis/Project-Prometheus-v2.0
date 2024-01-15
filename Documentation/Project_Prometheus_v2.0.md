# Introduction
One of the key requirements for a smart home is the safety and well-being of the inhabitants.  This is why it's important to have devices to actively monitor and rectify any threats that occur within the house. One such threat is the hazard of fire. According to the WHO, more than 300,000 deaths occur every year due to fire-related hazards [1].

Thus we see the importance of incorporating a fire monitoring and extinguishing system into the modern smart home. This should actively monitor the environment for critical changes in the levels of humidity, temperature and carbon dioxide and take necessary precautions to rectify any fires that get detected. The residents should also get the convenience of actively monitoring the state of the alarm and extinguishing system whenever they require it. This is the premise of the Prometheus Fire Alarm System v2.0.

## Prometheus v1.0
The idea of a fire alarm system has already been implemented in the form of Prometheus Fire Alarm System v1.0. This version aimed at developing a cost-effective monitoring and extinguishing system that is robust and scalable. The following were the targets of this project:
1. **Cost** - Develop a system based on off-the-shelf components that are cost-effective
2. **Scalable** - Availability for multiple monitoring devices to operate and synchronize in real-time
3. **Independent** - Whether connected or not, the device should monitor and react on its own without the involvement of the operator

## What's Next?
The next step would be to improve the Prometheus Fire Alarm System into a smart home device, bringing in the cost-effective, independent and scalable fire alarm and extinguishing system to the smart home. This will allow it to sit alongside other sensors and systems to provide all the smart homes with safety from fire.

The improvements thus suggested will be as follows.
1. Add a built-in charging system to allow the system to run when even the power is lost
2. Redesign the system to use the **nRF7002** chip instead of an ATMega
3. Rewrite the firmware to incorporate the Matter protocol
## Further Information
[Prometheus Fire Alarm System Firmware Repo](https://github.com/asankaSovis/prometheus-fire-alarm/tree/main)
[Prometheus Command Panel Repo](https://github.com/asankaSovis/prometheus-command-panel)
[Prometheus Fire Alarm System v1.0 Report](https://github.com/asankaSovis/prometheus-fire-alarm/blob/main/Additional%20Material/Engineering%20Design%20Project%20-%20Project%20Report.pdf)
[Prometheus Fire Alarm System Demo Video (Youtube)](https://youtu.be/5h3k6kufkHo?si=pq6S2FBMBYGGU_1R)

## References
[1] https://scholar.google.com/scholar_lookup?journal=MMWR:+Morbidity+and+Mortality+Weekly+Report&title=Prevention.+Deaths+resulting+from+residential+fires+and+the+prevalence+of+smoke+alarms--United+States,+1991-1995&author=CfD+Control&volume=47&issue=38&publication_year=1998&pages=803-6&pmid=9776167&
[2] https://www.co2meter.com/blogs/news/15164297-co2-gas-concentration-defined
[3] https://science.howstuffworks.com/environmental/earth/geophysics/fire.htm