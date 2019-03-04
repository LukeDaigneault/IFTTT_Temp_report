#include <DHT.h>

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#include "IFTTTWebhook.h"

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);



#define IFTTT_API_KEY "kKrijo3OTYZZfkHuStDs7pc9LIbjhaJXZyI093K7bB4"
#define IFTTT_EVENT_NAME "temp_received"
#define IFTTT_FINGERPRINT "AA:75:CB:41:2E:D5:F9:97:FF:5D:A0:8B:7D:AC:12:21:08:4B:00:8C"
IFTTTWebhook wh(IFTTT_API_KEY, IFTTT_EVENT_NAME, IFTTT_FINGERPRINT);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();

    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();


    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    dht.begin();

}

void loop() { // Wait a few seconds between measurements.
  delay(30000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  char buffer[10];
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C "));

  wh.trigger(dtostrf(t, 2, 2, buffer), dtostrf(h, 2, 2, buffer), dtostrf(hic, 2, 2, buffer));

}
