/*
使用u8g2显示数据
图形显示器：OpenJumper 12864 OLED
设备核心：SSD1306
*/
 
#include <U8g2lib.h>
#define rst  2
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 18, /* reset=*/ U8X8_PIN_NONE);


//取模得到的数组
const unsigned char col[] U8X8_PROGMEM= {0x00,0x00,0x00,0x08,0x80,0x07,0x60,0x01,0x80,0x1F,0xE0,0x00,0xFC,0x3F,0x20,0x00,
0xE0,0x0F,0x50,0x08,0xC8,0x0F,0x44,0x0A,0xC0,0x05,0x40,0x04,0xC0,0x03,0x00,0x00,
 };

 
void setup(void) {
  u8g2.begin();
}
 
void loop(void) {
  u8g2.clearBuffer();           // 清空显示设备内部缓冲区
  u8g2.drawXBMP( 32 , 0 , 16 , 16 , col );     //50,50为图片尺寸，根据你的图片尺寸修改
  u8g2.sendBuffer();         // 显示缓冲区内容
  delay(1000);  
}
