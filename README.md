# Sensor to Linux – ESP32 Ultrasonic Distance Logger
This project demonstrates how an ESP32 can read distance data from an HC-SR04 ultrasonic sensor and send it to a Linux server over Wi-Fi using a TCP connection. The system also uses three LEDs to indicate how close or far an object is — Red for near, Yellow for medium range, and Green for far.

## Project Overview
The ESP32 measures the distance using the ultrasonic sensor and transmits it to a Python server (`plot_server.py`) running on a local computer. The server receives the data in real time, making it possible to log, visualize, or process sensor readings on the host machine.

This project highlights:
- Sensor interfacing and timing using microcontrollers  
- Real-time communication over Wi-Fi sockets  
- Basic embedded feedback design with LEDs  

## Hardware Used
- ESP32 development board  
- HC-SR04 ultrasonic distance sensor  
- Three LEDs (Red, Yellow, Green)  
- Jumper wires and breadboard  
- Wi-Fi network for connectivity  

## Software and Tools
- PlatformIO (VS Code) for firmware development  
- Python 3 for server-side communication  
- Arduino libraries: `WiFi.h`, `Ultrasonic.h`

## How to Use
1. Open the file `src/main.cpp` in PlatformIO.  
2. Update your Wi-Fi credentials and server details:
   ```cpp
   const char* ssid = "_SSID_";
   const char* password = "_PASSWORD_";
   const char* serverIp = "PC_Local_ip";
   const int serverPort = 8888;
3. Build and upload the firmware to the ESP32.
4. On your Linux PC, run the Python server:
    python3 plot_server.py
5. Open the Serial Monitor in PlatformIO to view distance readings and LED status updates.


## Krishna Gorai
BS in Electronic Systems, IIT Madras
🌐 [LinkedIn](https://linkedin.com/in/krishna-gorai)