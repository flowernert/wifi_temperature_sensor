//DHT related inits and includes
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHT_DATA_PIN  D3
#define DHT_PWR_PIN   D2
#define DHTTYPE       DHT11

DHT_Unified dht(DHT_DATA_PIN, DHTTYPE);
int getTemperature();

//network related inits and includes
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define SSID = "your_ssid"; //put here your WiFi SSID
#define PASSWORD = "your_wifi_password"; //put here your WiFi password
#define ENDPOINT "http://192.168.0.35/temperatures" //replace the local IP with your server's one

int sendTemperature(int t);

//temperature sensors locations
#define ID_INSIDE 0
#define ID_SOUTH_SHADED 1
#define ID_SOUTH_SUN 2
#define ID_ATTIC 3

//current sensor definition
#define ID ID_ATTIC // customize with the ID of the sensor you're programming

#define INVALID_TEMPERATURE -200

//will send the temperature each 10 minutes
#define SLEEP_INTERVAL 10*60*1000*1000  // sleep interval is in µseconds

void setup() {

  int t = getTemperature();
  
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  sendTemperature(t);    

  ESP.deepSleep(SLEEP_INTERVAL);
}

void loop() {
}

int getTemperature() {
  //for DHT power supplying optimization
  pinMode(DHT_PWR_PIN, OUTPUT);
  digitalWrite(DHT_PWR_PIN, HIGH);

  //init
  dht.begin();
  delay(2000); //DHT11 bootup time

  //reading
  sensors_event_t event;  
  dht.temperature().getEvent(&event);

  //power supplying off
  digitalWrite(DHT_PWR_PIN, LOW);
  digitalWrite(DHT_DATA_PIN, LOW);

  if (isnan(event.temperature)) {
    //Serial.println("Impossible to read temperature");
    return INVALID_TEMPERATURE;
  }
  else {
    //Serial.println(("Temperature is ") + String(event.temperature));
    return event.temperature;
  }
}

int sendTemperature(int t) {
  if (WiFi.status() == WL_CONNECTED) { 
    StaticJsonBuffer<300> JSONbuffer;   
    JsonObject& jsonObj = JSONbuffer.createObject(); 
    jsonObj["id"] = ID;
    jsonObj["value"] = t;
    
    char jsonMsgBuffer[300] = {0};
    jsonObj.prettyPrintTo(jsonMsgBuffer, sizeof(jsonMsgBuffer));

    HTTPClient client;
    client.begin(ENDPOINT);
    client.addHeader("Content-Type", "application/json");
    int responseCode = client.PUT(jsonMsgBuffer);
    //String responseData = client.getString();
    //Serial.println("Response code is " + String(responseCode) + ", response data is " + responseData);
    client.end();
    return responseCode;
  }
  else {
    //Serial.println("WiFi unavailable");
    return -1;
  }
}

