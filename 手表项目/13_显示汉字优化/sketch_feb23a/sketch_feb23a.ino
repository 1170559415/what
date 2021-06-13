#include <FS.h>
uint8_t sdbuffer[5];
void setup() {
  Serial.begin(9600);
  SPIFFS.begin();
  File readfile ;
  readfile = SPIFFS.open("/Chinese.Ch", "r");
  int o = readfile.size();
  readfile.read(sdbuffer, 4);
  Serial.println("------------------------------");
  for (int i = 0; i < 4; i++)
  {
    Serial.print(sdbuffer[i] , HEX);
  }
  Serial.println();
  readfile.seek(8, SeekSet);
  readfile.read(sdbuffer, 4);
  for (int i = 0; i < 4; i++)
  {
    Serial.print(sdbuffer[i] , HEX);
  }
  Serial.println();
  //  Serial.println(millis());
  //  for (o; o > 0; o--) {
  //    //    readfile.seek(5, SeekSet);
  //    readfile.read(sdbuffer, 4);
  //    for (int i = 0; i < 4; i++)
  //    {
  //      Serial.print(sdbuffer[i] , HEX);
  //    }
  //  }
  //  Serial.println(millis());
}

void loop() {

}
