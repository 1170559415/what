#include <FS.h>

String string;

void setup() {
  Serial.begin(9600);
  SPIFFS.begin();
}

void loop() {
  File fs_ota;
  fs_ota = SPIFFS.open("/start.wifi","r+");
  fs_ota.print(millis());
  Serial.print(millis());
  delay(1000);
  ESP.restart();
}
