This section will discuss the overall technical specifications of the device. It includes each sensor/actuator/device categorized by what type of roll it performs along with the technical specifications. This section only includes the primary electronic items. For a comprehensive list of electronic components, please check the [Bill of Material](./Bill_of_Material). For operational details of the Prometheus Fire Alarm System, please refer to the [Operational Details](./Operational_Details) section.
# 1\. Control Unit
This is the brain of the system. It consists of an nRF7002dk which will be controlling the whole system. This also interfaces with all other components. With a dedicated nRF7002 Wi-Fi Companion IC and nRF5340 SoC, this is the ideal candidate for a project involving home automation and matter IoT device implementation.
## Specifications
### nRF7002 - Low-power, advanced security, seamless coexistence
- 2\.4GHz and 5GHz Dual-band
- Low-power and Secure Wi-Fi for the IoT
- Ideal coexistence with Bluetooth LE
- Supported in nRF Connect SDK
- Wi-Fi 6 Station (STA)
- Complies with 802.11a/b/g/n/ac/ax
- 1 Spatial Stream (SS)
- 20 MHz channel bandwidth
- 64 QAM (MCS7), 86 Mbps PHY throughput
- OFDMA (Downlink and Uplink)
- BSS Coloring
- TWT
- SPI / QSPI
- Co-existence interfaces
### nRF5340 SoC
- Dual-core Bluetooth 5.4 SoC supporting Bluetooth Low Energy, Bluetooth mesh, NFC, Thread and Zigbee
- 128 MHz Arm Cortex-M33 CPU with
- 1 MB Flash + 512 KB RAM
- 64 MHz Arm Cortex-M33 CPU with
- 256 KB Flash + 64 KB RAM
- Bluetooth Low Energy
- Bluetooth Direction Finding
- Bluetooth mesh
- Thread, Zigbee
- ANT
- NFC
- Advanced security
- USB, QSPI, HS-SPI
- 105 °C extended operating temperature
- 1\.7-5.5 V supply voltage range
# 2\. Fire Sensors
This is the module that directly interacts with the fire. This consists of two components as listed below.
## Specifications
### Smoke Sensor – Detect the existence of a fire
- Operating Voltage: 2.5V to 5.0V
- Power consumption: 150mA
- Detect/Measure: NH3, Nox, CO2, Alcohol, Benzene, Smoke
- Typical operating Voltage: 5V
- Digital Output: 0V to 5V (TTL Logic) @ 5V Vcc
- Analog Output: 0-5V @ 5V Vcc
### Temperature and Humidity Sensor – Detect the thermal changes in the surrounding
- Operating Voltage: 3.5V to 5.5V
- Operating current: 0.3mA (measuring) 60uA (standby)
- Output: Serial data
- Temperature Range: 0°C to 50°C
- Humidity Range: 20% to 90%
- Resolution: Temperature and Humidity both are 16-bit
- Accuracy: ±1°C and ±1%
They will sense the following three from the environment.
- Temperature – DHT11 sensor
- Humidity – DHT11 sensor
- Carbondioxide – MQ 135 sensor
DHT 11 communicates with the microcontroller digitally using UART while the MQ 135 senses analog and translates it to an SPI message via the ADC.
# 3\. Extinguisher
The extinguisher consists of three parts. It handles the extinguishing of the fire.
## Specifications
### 5V relay – Convers the 5V control signal to 12V so that the solenoid valve can operate
- Supply voltage: 3.75V to 6V
- Quiescent current: 2mA
- Current when the relay is active: \~70mA
- Relay maximum contact voltage: 250VAC or 30VDC
- Relay maximum current: 10A
### Solenoid Valve – Controls the water flow
- Material: Metal + plastic
- Voltage: DC 12V
- Power: 15W
- Current: 1.25A
- Inlet and outlet thread diameter: G1/2
- Pressure: 0.02 - 0.8Mpa
- Max fluid temperature: 80°C
- Operation mode: Normally Closed
### Nozzle – Sprays the water onto the fire so that the extinguishing is effective
While the relay and valve were bought, the nozzle was custom-fabricated to suit the needs.
# 4\. Indicators
The indicators give an indication to the people about the fire. We included two indicators, both visual and auditory for the best outcome.
## Specifications
### Visual – nRF7002dk Built-In LED
This LED indicates the technical status of the device. While initializing, the LED with be turned on. Once successful, it will flash every second to indicate that the device is actively monitoring. On an error, the LED will blink every second to indicate an error in the device.
### Visual – Indicator red LED
The red LED will blink twice every second to indicate that the system is operating normally. If it stays continuously on, then there is a warning or extinguishing has started. Any other combination or no light at all means that the device is turned off or is not working properly.
### Auditory – A 5V Buzzer
The buzzer stays off when the system is operating normally. It will beep once every second in case a warning is issued. It will then continuously buzz in case of an extinguishing.
