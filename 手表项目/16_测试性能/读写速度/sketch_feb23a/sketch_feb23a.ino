#include <FS.h>
#include <brzo_i2c.h>
#include <SSD1306Brzo.h>

/***********************************************引脚定义*******************************************************/
//-----------------------------------显示定义----------------------------------
SSD1306Brzo display(0x3c, D3, D4);
/*************************************************显示程序*************************************************/
//以下是汉字显示程序 (文件 , x , y, 是否换行)
void display_str(String string, int display_x , int display_y , bool line_feed) {
  int x = 0;
  int y = 0;
  int horizontal = 0;
  int vertical = 0;
  int display_bit = 0;
  String fs_name;
  String Chinese;

  File Fs_open;

  if (string.indexOf(':') != -1) {
    x = string.substring(string.indexOf(':') + 1, string.indexOf('.')).toInt();
    y = string.substring(string.indexOf('.') + 1, string.indexOf('$')).toInt();
    fs_name = "/";
    fs_name = fs_name + x + ".c";
    Fs_open = SPIFFS.open(fs_name, "r");
    string = Fs_open.readString();
    Fs_open.close();
    for (y ; y > 0; y--) {
      string = string.substring(string.indexOf('#') + 1);
    }
    string = string.substring(0, string.indexOf('#'));
  }
  else {
    Fs_open = SPIFFS.open(string, "r");
    string = Fs_open.readString();
    Fs_open.close();
  }

  while (string.indexOf('$') != -1) {
    x = string.substring(0, string.indexOf('^')).toInt();
    y = string.substring(string.indexOf('^') + 1, string.indexOf('$')).toInt();
    string = string.substring(string.indexOf('$') + 1);
    fs_name = "/";
    fs_name = fs_name + x + ".ch";
    Fs_open = SPIFFS.open(fs_name, "r");
    Chinese = Fs_open.readString();
    Fs_open.close();
    for (y ; y > 0; y--) {
      Chinese = Chinese.substring(Chinese.indexOf('$') + 1);
    }
    Chinese = Chinese.substring(0, Chinese.indexOf('$'));
    unsigned char display_char[24];
    for (int i = 0; i < 24; i++) {
      display_char[i] = Chinese.substring(0, Chinese.indexOf(',')).toInt();
      Chinese = Chinese.substring(Chinese.indexOf(',') + 1);
    }
    if (line_feed) {
      display.drawXbm(display_x + horizontal * 12 , display_y + vertical * 13 , 12, 12 , display_char);
    }
    else {
      display.drawXbm(display_x + horizontal * 12 , display_y , 12, 12 , display_char);
    }
    horizontal++;
    if (display_x + (horizontal + 1) * 12 > 128) {
      horizontal = 0;
      vertical++;
    }
  }
}

//初始化主程序
void setup() {
  SPIFFS.begin();
  display.init();
  display.flipScreenVertically();
  Serial.begin(9600);
}

void loop() {
  Serial.print("开始时间 :");
  Serial.print(millis());
  display.clear();
  display_str("/fighting.day", 0, 0, false);
  display.display();
  Serial.print("  结束时间 :");
  Serial.println(millis());
}
