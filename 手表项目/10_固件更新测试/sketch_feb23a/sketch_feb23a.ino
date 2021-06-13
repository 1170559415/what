#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* host = "updata";
const char* ssid = "xwbtd";
const char* password = "a12345678b.";

//IPAddress staticIP(192, 168, 2, 11);
//IPAddress gateway(192, 168, 2, 1);
//IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void) {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
//  WiFi.config(staticIP, gateway, subnet);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  MDNS.begin(host);

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  Serial.println(WiFi.localIP().toString());

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
}

void loop(void) {
  httpServer.handleClient();
}
