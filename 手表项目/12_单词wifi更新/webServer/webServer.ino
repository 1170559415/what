#include <ESP8266WiFi.h>

const char* ssid = "xwbtd";
const char* password = "a12345678b.";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");

    String req = client.readStringUntil('\r');
    String s;
    Serial.println(req);
    client.flush();
    if (req.indexOf("/Fs=") != -1) {
      String Word_name = req.substring(req.indexOf('=')+1, req.indexOf('$'));
      s += Word_name;
      s += "\r\n\r\n";
      req = req.substring(req.indexOf('$') + 1);
      //      File Fs_open;
      //      if (SPIFFS.exists(Word_name)) {
      //        SPIFFS.remove(Word_name);
      //      }
      //      Fs_open = SPIFFS.open(Word_name , "a+");
      while (req.indexOf('$') != -1) {
        String Word_data = req.substring(0, req.indexOf('$'));
        //        Fs_open.println(Word_data);
        req = req.substring(req.indexOf('$') + 1);
        Serial.println(Word_data);
        Word_data += "\r\n";
//        Word_data += '\n';
        s += Word_data;
      }
      //      Fs_open.close();
    }
//    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nHello world ! ";

    client.print(s);
    delay(1);
    Serial.println("Client disonnected");
  }
}
