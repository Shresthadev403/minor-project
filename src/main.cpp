#include <Arduino.h>

// #include<ESP8266WiFi.h>


// my custom header files
#include<headers/Sound.h>
#include<headers/Internet.h>


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


}

void loop () {
sendPostRequest();
 playBuzzer(active_buzzer,1,1000); 
 delay(10000);



}