#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>

#include <StringSplitter.h>
// for gps portion
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// nfc read write
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

// custom headers
#include <headers/Internet.h>
#include <headers/Sound.h>
// #include <headers/Display.h>
#include <headers/Gps.h>
#include <headers/File.h>

// this is for pn532 declaration
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
String tagId = "None";
byte nuidPICC[4];

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c // initialize with the I2C addr 0x3C Typically eBay OLED's
// #define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// Replace with your network credentials
const char *ssid = "123";            // Replace with your own SSID
const char *password = "1234567890"; // Replace with your own password

const int ledPin = 2;
String ledState;


static const int SoundPin=D0;

static const int RXPin = D5, TXPin = D6;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
unsigned long lastRead; // variable to store the time of the last GPS reading
// unsigned long interval = 5000; // interval between GPS readings in milliseconds

bool tagIdLocallyCheck(String tagId)
{

  bool isFound = false;
  String str;
  File file = LittleFS.open("/tags.txt", "r");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return false;
  }

  // Serial.println("File Content:");
  while (file.available())
  {
    // Serial.write(file.read());
    str = file.readString();
  }
  file.close();

  StringSplitter *splitter = new StringSplitter(str, ',', 50); // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
  Serial.println("Item count: " + String(itemCount));
  String newstr = "";
  for (int i = 0; i < itemCount; i++)
  {

    String item = splitter->getItemAtIndex(i);
    Serial.println("Item @ index " + String(i) + ": " + String(item));
    if (String(item) == tagId)
    {
      isFound = true;
    }
    else
    {
      if (i == 0)
      {
        newstr = String(item);
      }
      else
      {
        newstr = newstr + ",";
        newstr = newstr + String(item);
      }
    }
  }

  // Serial.println(newstr);
  //   Serial.println(str);
  File filenew = LittleFS.open("/tags.txt", "w");
  if (!filenew)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }
  if (isFound == true)
  {
    filenew.print(newstr);
  }
  else
  {
    str = str + tagId + ",";
    filenew.print(str);
  }
  filenew.close();
  Serial.println(newstr);
  Serial.println(str);

  return isFound;
}

// gps
String getGpsLocation()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    // Serial.print(gps.location.lat(), 6);
    // Serial.print(F(","));
    // Serial.print(gps.location.lng(), 6);
    String latndLng;
    float lat = gps.location.lat();
    char charData[20];
    sprintf(charData, "%.6f", lat);
    // Serial.println(charData);
    latndLng = String(charData);
    latndLng = latndLng + ",";
    // Serial.println(latndLng);
    // Serial.println("and");
    float lng = gps.location.lng();
    sprintf(charData, "%.6f", lng);
    latndLng = latndLng + String(charData);

    Serial.println(latndLng);
    return latndLng;
  }
  else
  {
    Serial.print(F("INVALID"));
    return "INVALID";
  }
}

////// nfc portion/ thus is for nfc
void readNFC()
{

  if (nfc.tagPresent())
  {

    NfcTag tag = nfc.read();

    tag.print();
    tagId = tag.getUidString();
    Serial.println(tagId);

    display.setCursor(0, 0);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.println("tag found");

    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);


    
// this is for user checkin and checkout request
 bool dataFound = tagIdLocallyCheck(tagId);
  Serial.print(dataFound);
  if(dataFound==true){
    display.setCursor(1, 20);
    display.setTextSize(1);
    display.print("check Out");

    display.setCursor(2, 40);
    display.setTextSize(1);
    display.print(tagId);
    display.display();
    
sendUserTravelCheckOut(tagId);
 playBuzzer(D7,2,100);

    // display.display();
  }else{
     display.setCursor(1, 20);
    display.setTextSize(1);
    display.print("check In");


display.setCursor(2, 40);
    display.setTextSize(1);
    display.print(tagId);
    display.display();

    sendUserTravelCreate(tagId);
     playBuzzer(D7,3,500);
   
    // display.display();
  }

  // display.setCursor(2, 40);
  //   display.setTextSize(1);
  //   display.print(tagId);
  //   display.display();

    display.clearDisplay();

  
  }

  delay(1000);
}

void setup()
{

  Serial.begin(9600);
  pinMode(D7,OUTPUT);
  display.begin(i2c_Address, true);
  delay(250);
  display.clearDisplay();
  display.display();
  nfc.begin();

  // gps portion
  ss.begin(GPSBaud);

  // led portion
  pinMode(ledPin, OUTPUT);

  // Initialize LittleFS
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    // playBuzzer(SoundPin,2,1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // creating mobile hotspot;

  // WiFi.softAP("MyHotspot", "password");
  // IPAddress myIP = WiFi.softAPIP();
  // Serial.print("AP IP address: ");
  // Serial.println(myIP);

  // / nfc portion

  display.begin(i2c_Address, true);
  delay(250);
  display.clearDisplay();
  display.display();
  nfc.begin();
}

unsigned long previousMillis = 0; // variable to store the last time the event was triggered
const long interval = 5000;       // interval in milliseconds


unsigned long lastSendTime = 0;
unsigned long sendInterval = 5000;

void loop()
{
  
 

  //  while (ss.available()>0 )
  // {
  //     Serial.println("ss.available");

  //   if (gps.encode(ss.read())>0)
  //   {
  //     Serial.println("ss.read");
  //   String data=getGpsLocation();
  //   if(data=="INVALID"){
  //     break;
  //   }else{
  //     Serial.println("valid");
  //   // Send GPS location to server
  //     if (millis() % 5000 == 0) {
  //       // sendGPSData(lat, lng);
  //       Serial.println('send gps data to server');
  //     }
  //     Serial.println("sending data to server");
  //     // sendPostGPSLocationRequest(data);
  //     // delay(5000);
  //     // break;
  //   }
  //     Serial.println(data);

  //   }
  // }

  // for nfc and display
  // readNFC();
  // display.display();
  // delay(2000);
  // display.clearDisplay();



   while (ss.available()) { // while there is data available from the GPS module
    gps.encode(ss.read()); 
    // read the data and feed it to the TinyGPS++ object
    // Serial.println("ddd");
  }

 
    // Serial.println("10s passed");
     // check if the current time is a multiple of 5000ms
    if (gps.location.isValid()) { // check if the GPS has a valid fix
      // Serial.print("Latitude: ");
      // Serial.println(gps.location.lat(), 6); // print the latitude with 6 decimal places
      // Serial.print("Longitude: ");
      // Serial.println(gps.location.lng(), 6); 
    //   Serial.println(millis());
    //    if (millis() % 10000 == 0) {
    // String data=getGpsLocation();
    // Serial.println(data);
    // sendPostGPSLocationRequest(data);


    //    }else{
    //     Serial.print("ddddddddddddd");
    //     delay(2000);
    //    }

     unsigned long currentTime = millis();

    //  Serial.println(lastSendTime);
    //  Serial.println(currentTime);

      // for nfc and display
  readNFC();
  display.display();
  delay(2000);
  display.clearDisplay();

  if (currentTime - lastSendTime >= sendInterval) {
    // String gpsLocation = getGpsLocation();
    // sendPostGPSLocationRequest(gpsLocation);
    String data=getGpsLocation();
    Serial.println(data);
    sendPostGPSLocationRequest(data);
    lastSendTime = currentTime;
  }
      
      // print the longitude with 6 decimal places



    } else {
      Serial.println("No GPS data available.");
    }
    // Serial.println("another");
    // for nfc and display
  // readNFC();
  // display.display();
  // delay(2000);
  // display.clearDisplay();
  

  // bool data = tagIdLocallyCheck("sdfsdfsdcdc dfs");
  // Serial.print(data);
  // delay(5000);
















  









  // This sketch displays information every time a new sentence is correctly encoded.

  // unsigned long currentMillis = millis();  // get the current time

  // if (currentMillis - previousMillis >= interval) {
  //   // if 5 seconds have passed
  //   previousMillis = currentMillis;  // update the previous time
  //   // do something here, e.g. send a message via Wi-Fi
  //   Serial.println("5 seconds have passed");
  // }

 

// while (gpsSerial.available()) {
//     if (gps.encode(gpsSerial.read())) {
//       double lat, lng;
//       gps.f_get_position(&lat, &lng);

//       // Send GPS location to server
//       if (millis() % 5000 == 0) {
//         // sendGPSData(lat, lng);
//       }
//     }
  // }




  // Serial.println("dd");

  // if (millis() > 5000 && gps.charsProcessed() < 10)
  // {
  //   Serial.println(F("No GPS detected: check wiring."));
  //   while (true)
  //     ;
  // }
}

/// the above is the gps reading

// calculate the time since the last GPS reading
// unsigned long now = millis();
// unsigned long timeSinceLastRead = now - lastRead;
// check if the interval has passed
// if (timeSinceLastRead >= interval)
// {
//     // if the interval has passed, update the lastRead variable and repeat the process
//     lastRead = now;
// }
// else
// {
//     // if the interval has not passed, delay for the remaining time
//     delay(interval - timeSinceLastRead);
// }
// delay(5000);