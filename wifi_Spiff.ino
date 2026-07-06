#include <ESP8266WiFi.h>                                                    //provide the things for esp to connect to wifi
#include <WiFiClientSecureBearSSL.h>                                        //
#include <ESP8266HTTPClient.h>                                              //provide This library sends HTTP and HTTPS requests.
#include <LittleFS.h>                                                       //This library provides a file system inside the ESP8266's flash memory.

//=========================
// WiFi Configuration
//=========================
const char* ssid = "Hii";
const char* password = "mack12345";

// HTTPS File URL
const char* fileURL = "https://www.gnu.org/software/hello/manual/hello.txt";    //from where to download

// File stored in Flash
const char* filePath = "/hello.txt";                                            //where to download

//=========================
// Configuration
//=========================
#define BUF_SIZE      1024                                                      //1kb buffer size
#define DL_TIMEOUT    15000                                                     //15sec timeout for connectio request for http
#define WIFI_TIMEOUT  10000                                                     //10sec timeout for wificonnection timeout

bool wifiConnect();                                                             //wifi connection code written
bool fetchFile();                                                               //fetch file code written

void setup()                                                                    //setup function
{
    Serial.begin(115200);                                                       //begin the communicttion between device wth same baoutd rate
    delay(500);                                                                 //delay of 0.5msec for connection setup for uart communcation

    Serial.println();
    Serial.println("=================================");
    Serial.println(" ESP8266 HTTPS Downloader");
    Serial.println("=================================");

    // Mount LittleFS
    if (!LittleFS.begin())                                                      //calling the littlefs.begin() function for "It makes the connection between your program and the LittleFS file system stored in the flash memory."
    {
        Serial.println("LittleFS Mount Failed!");
        return;
    }

    Serial.println("LittleFS Mounted Successfully");

    if (!wifiConnect())                                                         //calling the wifi connection function for connection before fetching file
        return;

    fetchFile();                                                                //after wifi connect calling the fetch function to fetch file from that file
}

void loop()                                                                     //do nothing just for looping the whole program continously prefrom the task
{
}

//------------------------------------------------
// Connect to WiFi
//------------------------------------------------
bool wifiConnect()
{
    Serial.print("Connecting to WiFi");

    WiFi.mode(WIFI_STA);                                                        //calling the wifi mode for wifi station mode and in that it do "ESP8266, behave like a normal Wi-Fi client and connect to a router."
    WiFi.begin(ssid, password);                                                 //for connecting means in interanl register writing we pass the ssid and password 

    unsigned long start = millis();                                             //calling millis() function to get the time from timer that this instance later use for calculating time out

    while (WiFi.status() != WL_CONNECTED)                                       //calling the wifi status is to check wifi connected ro not and if not then this loop and 
        {
        delay(500);                                                             //delay for wifi status checking
        Serial.print(".");

        if (millis() - start > WIFI_TIMEOUT)                                    //checking is wifi connection timeout passed or not
        {
            Serial.println();
            Serial.println("WiFi Connection Timeout");                          //if timeout passed then print the wifi timeout
            return false;
        }
    }

    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    return true;
}

//------------------------------------------------
// Download HTTPS File
//------------------------------------------------
bool fetchFile()
{
    std::unique_ptr<BearSSL::WiFiClientSecure> client(                          //
        new BearSSL::WiFiClientSecure);

    client->setInsecure();                                                      //we set to insecure to avoid the certification

    HTTPClient https;                                                           //declaring the https object of class HTTPCient

    https.setTimeout(DL_TIMEOUT);                                               //calling the setTimeout object to det the timeout time

    Serial.println();
    Serial.println("Connecting to Server...");

    if (!https.begin(*client, fileURL))                                         //calling the begin object of https class to set the client to donwload from that url
    {
        Serial.println("HTTPS Begin Failed");
        return false;
    }

    int httpCode = https.GET();                                                 //passing get request to the URL and it reture the constant

    if (httpCode != HTTP_CODE_OK)                                               //check the connection request
    {
        Serial.printf("HTTP GET Failed : %d\n", httpCode);                      //if faild then print the code
        https.end();                                                            //if failed then end the https request connection
        return false;
    }

    if (LittleFS.exists(filePath))                                              //check if the file present or not in the path if yes then remove it
        LittleFS.remove(filePath);  
                                                                                //"LittleFS.exists(filePath) checks whether the specified file is already present in the LittleFS 
                                                                                //file system. If it exists, LittleFS.remove(filePath) deletes the old file before creating and 
                                                                                //writing a new one. This ensures that the newly downloaded file replaces any previous version cleanly."

    File file = LittleFS.open(filePath, "w");                                   //open that availabe in write mode

    if (!file)                                                                  //if file open or not
    {
        Serial.println("Cannot Open File");
        https.end();                                                            //if file not open then end the connection of http
        return false;
    }

    WiFiClient *stream = https.getStreamPtr();

    uint8_t buffer[BUF_SIZE];                                                   //create a buffer or BUF_SIZE for storing the incoming data

    size_t totalBytes = 0;

    unsigned long downloadStart = millis();

    while (https.connected() || stream->available())                            //if http connected and stream is availble for taransfer
    {
        size_t available = stream->available();

        if (available)
        {
            if (available > BUF_SIZE)
                available = BUF_SIZE;

            unsigned long t1 = micros();

            int bytesRead = stream->readBytes(buffer, available);

            if (bytesRead <= 0)
                break;

            file.write(buffer, bytesRead);

            unsigned long t2 = micros();

            float speed =
                (bytesRead / 1024.0) /
                ((t2 - t1) / 1000000.0);

            totalBytes += bytesRead;

            Serial.printf(
                "Chunk : %4d Bytes | Speed : %7.2f KB/s | Total : %6u Bytes\n",
                bytesRead,
                speed,
                totalBytes);
        }
        else
        {
            delay(1);
        }
    }

    file.close();
    https.end();

    unsigned long elapsed = millis() - downloadStart;

    float avgSpeed =
        (totalBytes / 1024.0) /
        (elapsed / 1000.0);

    Serial.println();
    Serial.println("=================================");
    Serial.println("Download Complete");
    Serial.println("=================================");
    Serial.printf("Saved File : %s\n", filePath);
    Serial.printf("Total Size : %u Bytes\n", totalBytes);
    Serial.printf("Time       : %.2f sec\n", elapsed / 1000.0);
    Serial.printf("Average    : %.2f KB/s\n", avgSpeed);

    Serial.println();

    // Read the downloaded file
    File f = LittleFS.open(filePath, "r");                                  

    if (f)
    {
        Serial.println("========== File Content ==========");

        while (f.available())
            Serial.write(f.read());

        f.close();

        Serial.println();
        Serial.println("==================================");
    }

    return true;
}

//Note : BearSSL is a lightweight SSL/TLS library.