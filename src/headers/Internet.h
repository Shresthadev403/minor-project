#include <ESP8266WiFi.h> // static wifi connect
#include <DNSServer.h>

// string

// #include <ESP8266WebServer.h>
// #include <WiFiManager.h>

// for making http request
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
// for parsing json
#include <ArduinoJson.h>

// needed for wifi manager
// #include <ESPAsyncWiFiManager.h>

// WiFiManager wifiManager;
// WiFiClient client;

// HTTPClient http;

// connect to the wifi at compile time
void connectToWiFi(String ssid, String password)
{
  WiFi.mode(WIFI_STA); // make nodemcu as a station(eg .as a mobile)
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("connecting....");
    Serial.println(WiFi.status());
    delay(300);
  }
  Serial.println("connected sucessfully");
  Serial.print("Ip address:");
  Serial.println(WiFi.localIP()); // ip of nodemcu

  Serial.print("Mac address:");
  Serial.println(WiFi.macAddress()); // mac address of nodemcu
}

// connect to wifi during runtime using soft access point

// void connectWifiRuntime()
// {
//     wifiManager.autoConnect("connect nodemcu");
//     Serial.println("connected...to the wifi node mcu :)");
// }

// void sendGetRequest(String url)
// {
//     if (WiFi.status() == WL_CONNECTED)
//     {
//         int httpCode;
//         http.begin(client, url);
//         Serial.println(url);
//         Serial.println("waitin for response");
//         httpCode = http.GET();
//            Serial.println(httpCode);

//         if (httpCode > 0)
//         {
//             String payload = http.getString();
//             Serial.println(httpCode);
//             Serial.println(payload);
//             Serial.println("data sent sucessfully");

//             const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 64;
//             DynamicJsonDocument doc(capacity);

//             // Parse JSON object
//             // DeserializationError error = deserializeJson(doc, payload);
//             // if (error)
//             // {
//             //     Serial.print(F("deserializeJson() failed: "));
//             //     Serial.println(error.f_str());

//             //     return "error";
//             // }
//             // Serial.println("..............................");
//             // int userId = doc["userId"]; // 1
//             //    Serial.println(userId);
//             // return payload;
//         }
//         else
//         {
//             Serial.println("error in httpscode with status code" + httpCode);

//         }
//         http.end();
//     }
//     else
//     {
//         Serial.println("wifi-disconnected");

//     }
// }

void sendPostGPSLocationRequest(String locationVar)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    // Ignore SSL certificate validation
    client->setInsecure();

    HTTPClient https;
    // Your Domain name with URL path or IP address with path
    https.begin(*client, "https://1a5b-202-51-80-210.ngrok.io/update/bustravel");

    // If you need Node-RED/server authentication, insert user and password below
    // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

    // Specify content-type header
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", "Bearer eyxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxJ");
    String macAddressVar = WiFi.macAddress();
    // Data to send with HTTP POST
    String httpRequestData = "{\"macAddress\":\"macAddressVar\",\"location\":\"locationVar\",\"password\":\"A1234567@?##\"}";
    // Send HTTP POST request
    httpRequestData.replace("macAddressVar", String(macAddressVar));
    httpRequestData.replace("locationVar", String(locationVar));
    Serial.println(httpRequestData);
    int httpResponseCode = https.PUT(httpRequestData);

    // If you need an HTTP request with a content type: application/json, use the following:
    // http.addHeader("Content-Type", "application/json");
    // int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

    // If you need an HTTP request with a content type: text/plain
    // http.addHeader("Content-Type", "text/plain");
    // int httpResponseCode = http.POST("Hello, World!");

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = https.getString();
    Serial.println(payload);

    // Free resources
    https.end();
    // delay(5000);
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}




void sendUserTravelCreate(String tagIdVar)
{
  Serial.println("user travel create");
  if (WiFi.status() == WL_CONNECTED)
  {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    // Ignore SSL certificate validation
    client->setInsecure();

    HTTPClient https;
    // Your Domain name with URL path or IP address with path
    https.begin(*client, "https://fcb7-202-51-80-210.ngrok.io/create/usertravel");

    // If you need Node-RED/server authentication, insert user and password below
    // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

    // Specify content-type header
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", "Bearer eyxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxJ");
    String macAddressVar = WiFi.macAddress();
    // Data to send with HTTP POST
    String httpRequestData = "{\"macAddress\":\"macAddressVar\",\"tagId\":\"tagIdVar\",\"password\":\"A1234567@?##\"}";
    // Send HTTP POST request
    httpRequestData.replace("macAddressVar", String(macAddressVar));
    httpRequestData.replace("tagIdVar", String(tagIdVar));
    Serial.println(httpRequestData);
    int httpResponseCode = https.POST(httpRequestData);

    // If you need an HTTP request with a content type: application/json, use the following:
    // http.addHeader("Content-Type", "application/json");
    // int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

    // If you need an HTTP request with a content type: text/plain
    // http.addHeader("Content-Type", "text/plain");
    // int httpResponseCode = http.POST("Hello, World!");

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = https.getString();
    Serial.println(payload);

    // Free resources
    https.end();
    // delay(5000);
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}





void sendUserTravelCheckOut(String tagIdVar)
{
  Serial.println("user travel checkout");

  if (WiFi.status() == WL_CONNECTED)
  {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    // Ignore SSL certificate validation
    client->setInsecure();

    HTTPClient https;
    // Your Domain name with URL path or IP address with path
    https.begin(*client, "https://fcb7-202-51-80-210.ngrok.io/usertravel/checkout");

    // If you need Node-RED/server authentication, insert user and password below
    // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

    // Specify content-type header
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", "Bearer eyxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxJ");
    String macAddressVar = WiFi.macAddress();
    // Data to send with HTTP POST
    String httpRequestData = "{\"macAddress\":\"macAddressVar\",\"tagId\":\"tagIdVar\",\"password\":\"A1234567@?##\"}";
    // Send HTTP POST request
    httpRequestData.replace("macAddressVar", String(macAddressVar));
    httpRequestData.replace("tagIdVar", String(tagIdVar));
    Serial.println(httpRequestData);
    int httpResponseCode = https.PUT(httpRequestData);

    // If you need an HTTP request with a content type: application/json, use the following:
    // http.addHeader("Content-Type", "application/json");
    // int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

    // If you need an HTTP request with a content type: text/plain
    // http.addHeader("Content-Type", "text/plain");
    // int httpResponseCode = http.POST("Hello, World!");

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = https.getString();
    Serial.println(payload);

    // Free resources
    https.end();
    // delay(5000);
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}



