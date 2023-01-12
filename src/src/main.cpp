#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

//custom headers
#include <headers/Internet.h>
#include<headers/Sound.h>
// Replace with your network credentials
const char* ssid = "ALHN-52C0"; //Replace with your own SSID
const char* password = "66954015"; //Replace with your own password

const int ledPin = 2;
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



void setup(){

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Initialize LittleFS
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // // Connect to Wi-Fi
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi..");
  // }

  // // Print ESP32 Local IP Address
  // Serial.println(WiFi.localIP());

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

 

  // Start server
  server.begin();
}
 
void loop(){
  
}