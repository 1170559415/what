/**
 * 功能描述：演示文件读写功能
 */
#include <FS.h>
 

void setup(){
  Serial.begin(9600);
  
  File myFile;
  //打开文件 可读
   
  myFile = SPIFFS.open("cs.c","r");
  Serial.write(myFile.read());
  myFile.close();
}
 
void loop(){
}
