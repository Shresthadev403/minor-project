#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>


#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

//custom headers
#include <headers/Internet.h>
#include<headers/Sound.h>
#include<headers/Display.h>
#include<headers/Gps.h>
// Replace with your network credentials
const char* ssid = "123"; //Replace with your own SSID
const char* password = "1234567890"; //Replace with your own password

const int ledPin = 2;
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


// // for gps portion
// static const int RXPin = 4, TXPin = 3;
// static const uint32_t GPSBaud = 96000;

// // The TinyGPSPlus object
// TinyGPSPlus gps;

// // The serial connection to the GPS device
// SoftwareSerial ss(RXPin, TXPin);



void setup(){

  Serial.begin(9600);

  // gps portion 
    // ss.begin(GPSBaud);

    // led portion
  pinMode(ledPin, OUTPUT);

  // Initialize LittleFS
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // creating mobile hotspot;


   WiFi.softAP("MyHotspot", "password");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  

  //sending post request
  // sendPostRequest();
  firstsetup();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  // Route to load style.css file
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.js");
  });

  server.on("/route", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/indexRoute.html");
  });

   // Route to load style.css file
  server.on("/indexRoute.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/indexRoute.js");
  });

 

  // Start server
  server.begin();
}
 
void loop(){
 String data= getGpsLatAndLng();
 Serial.println(data);
 Serial.println("sending to server");
 sendPostRequest(data);
  // delay(2000);
}