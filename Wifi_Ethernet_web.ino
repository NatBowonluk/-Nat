/****************************************************************************************************************************
  WiFi_Ethernet_Complex_ESP32.ino
  EthernetWebServer is a library for the Ethernet shields to run WebServer

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/EthernetWebServer
  Licensed under MIT license
 ***************************************************************************************************************************************/

#include <WiFi.h>
#include <WebServer.h>

#include <SPI.h>
#include <EthernetWebServer.h>

const char* ssid     = "RD.FL4";
const char* password = "28Onnut88";

EthernetWebServer ethernetServer(80);
WebServer wifiServer(80);

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

typedef struct {
  int8_t w_ip1, w_ip2, w_ip3, w_ip4;
  int8_t w_sub1, w_sub2, w_sub3, w_sub4;
  int8_t w_gw1, w_gw2, w_gw3, w_gw4;
  int8_t l_ip1, l_ip2, l_ip3, l_ip4;
} WLAN;
WLAN wlan;

void spitLan(String ip_ ) {
  String  t1, t2, t3, t4  ;
  String getIP;
  int count , len ;
  t1 = ip_;

  len = t1.length();
  count = t1.indexOf(".", 0);
  getIP = t1.substring(0, count);
  t2 = t1.substring(count, len);
  wlan.l_ip1 = getIP.toInt();

  len = t2.length();
  count = t2.indexOf(".", 1);
  getIP = t2.substring(1, count);
  t3 = t2.substring(count, len);
  wlan.l_ip2 = getIP.toInt();

  len = t3.length();
  count = t3.indexOf(".", 1);
  getIP = t3.substring(1, count);
  t4 = t3.substring(count, len);
  wlan.l_ip3 = getIP.toInt();

  len = t4.length();
  count = t4.indexOf(".", 1);
  getIP  = t4.substring(1, count);
  wlan.l_ip4 = getIP.toInt();
}

void handleEthernetRoot() {
  ethernetServer.send(200, "text/plain", "Hello from ESP32 Ethernet!");
}

void handleWiFiRoot() {
  wifiServer.send(200, "text/plain", "Hello from ESP32 WiFi!");
}

void setup()
{
  Serial.begin(115200);
  Serial.println("\nStarting WiFi_Ethernet_Complex_ESP32 on " + String(ARDUINO_BOARD));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to");
  String wifilocal = WiFi.localIP().toString().c_str();
  spitLan(wifilocal);


  Ethernet.init(5);

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  // uint16_t index = millis() % NUMBER_OF_MAC;

  IPAddress ETHip(wlan.l_ip1, wlan.l_ip2, wlan.l_ip3, wlan.l_ip4);
  Ethernet.begin(mac, ETHip);
  Serial.print(F("HTTP EthernetWebServer is @ IP : "));
  Serial.println(Ethernet.localIP());
  ethernetServer.on("/", handleEthernetRoot);
  ethernetServer.begin();

  Serial.print(F("HTTP WiFiWebServer is @ IP : "));
  Serial.println(WiFi.localIP());
  wifiServer.on("/", handleWiFiRoot);
  wifiServer.begin();

}

void loop()
{
  ethernetServer.handleClient();
  wifiServer.handleClient();
  delay(1000);
}
