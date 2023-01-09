#include <Arduino.h>

// #include<ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// my custom header files
#include<headers/Sound.h>
#include<headers/Internet.h>

//server instance
ESP8266WebServer server(80);


const  unsigned  char active_buzzer= D5;
void setup () {
  Serial.begin(9600); 
    while (!Serial) continue;                 
  Serial.print("the program is going to start");  

  // conecting to a wifi network
//  connectToWiFi("ALHN-52C0","66954015");

// connect to wifi during runtime dynamically
connectWifiRuntime();

  // playing sound
 pinMode(active_buzzer,OUTPUT); // configure output for sound

//send get request to thinkspeak channel server
// sendGetRequest(" http://c305-202-51-69-31.ngrok.io/products ");



// create a server on nodemcu for handle ing webpage

 server.on(F("/"), []() {
    server.send(200, "text/html",getWebPage() );
  });
  
  server.begin();
   Serial.println("HTTP server started");



}

void loop () {
// sendPostRequest();


//handle request on server of nodemcu
server.handleClient();
 playBuzzer(active_buzzer,1,1000); 
 delay(10000);



}