# ESP8266 HTTPS File Downloader using BearSSL and LittleFS

## Overview

This project is an ESP8266-based HTTPS file downloader developed using the Arduino framework. It securely downloads files from a cloud server over HTTPS and stores them in the ESP8266's internal flash memory using LittleFS without requiring external storage.

The project demonstrates secure communication using BearSSL, HTTP file downloading, buffered data transfer, and flash-based file management for IoT and embedded applications.

---

## Features

* Connects ESP8266 to a Wi-Fi network
* Downloads files securely over HTTPS
* Uses BearSSL for SSL/TLS encrypted communication
* Stores downloaded files in LittleFS
* Automatically replaces existing files
* Buffered file transfer for efficient memory usage
* Displays download progress and transfer speed
* Reads and prints downloaded file contents through the Serial Monitor

---

## How to Compile and Run

1. Open the project in Arduino IDE.
2. Install the ESP8266 Board Package.
3. Install the required libraries.
4. Update the Wi-Fi credentials and HTTPS file URL.
5. Select the ESP8266 board and COM port.
6. Upload the program to the ESP8266.
7. Open the Serial Monitor (115200 baud) to view the download process.

---

## Required Libraries

| Library | Purpose |
| -------- | ------- |
| ESP8266WiFi | Wi-Fi connectivity |
| WiFiClientSecureBearSSL | Secure HTTPS communication |
| ESP8266HTTPClient | HTTP/HTTPS requests |
| LittleFS | Internal flash file system |

---

## Configuration

Update the following parameters before uploading the program.

### Wi-Fi Credentials

```cpp
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_Password";
```

### HTTPS File URL

```cpp
const char* fileURL = "https://example.com/file.txt";
```

### File Storage Path

```cpp
const char* filePath = "/hello.txt";
```

---

## Working Principle

1. Initializes the Serial Monitor.
2. Mounts the LittleFS file system.
3. Connects the ESP8266 to the Wi-Fi network.
4. Creates a secure HTTPS client using BearSSL.
5. Sends an HTTP GET request to the specified server.
6. Downloads the file in buffered chunks.
7. Stores the downloaded data into LittleFS.
8. Closes the HTTPS connection.
9. Reads and displays the stored file through the Serial Monitor.

---

## Requirements

* ESP8266 Development Board (NodeMCU/Wemos D1 Mini)
* Arduino IDE
* ESP8266 Board Package
* Wi-Fi Connection
* USB Cable

---

## Key Concepts Used

* Embedded C++
* ESP8266 Wi-Fi Programming
* HTTPS Communication
* SSL/TLS (BearSSL)
* HTTP GET Requests
* LittleFS File System
* Flash Memory Management
* Buffered File Transfer
* Serial Communication
* Pointer and Stream Handling

---

## Limitations

* Certificate verification is disabled (`setInsecure()`)
* Supports downloading one file at a time
* Requires an active Wi-Fi connection
* Limited by the ESP8266's available flash memory

---

## Future Improvements

* Certificate Validation
* OTA Firmware Updates
* Resume Interrupted Downloads
* Download Progress Percentage
* SHA-256 File Integrity Verification
* JSON Configuration File Support
* Automatic Retry Mechanism

---

## Applications

* IoT Device Configuration
* Edge Device File Management
* Firmware Distribution
* Secure Configuration Downloads
* Cloud-Based Embedded Systems
* Smart Home Devices
* Remote Data Logging

---

## Project Workflow

```
Power ON
    │
    ▼
Initialize Serial Monitor
    │
    ▼
Mount LittleFS
    │
    ▼
Connect to Wi-Fi
    │
    ▼
Create Secure HTTPS Client
    │
    ▼
Send HTTP GET Request
    │
    ▼
Download File in Chunks
    │
    ▼
Store File in LittleFS
    │
    ▼
Display Download Statistics
    │
    ▼
Read and Print File Content
```

---

## Author

**Mukesh Choudhary**

Electronics & Telecommunication Engineering

Interested in Embedded Systems, Firmware Development, IoT, and Robotics.

---

## License

This project is open-source and available under the MIT License.
