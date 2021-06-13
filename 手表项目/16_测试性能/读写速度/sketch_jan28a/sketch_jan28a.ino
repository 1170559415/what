#include <FS.h>
#include <brzo_i2c.h>
#include <SSD1306Brzo.h>

/***********************************************引脚定义*******************************************************/
//-----------------------------------显示定义----------------------------------
SSD1306Brzo display(0x3c, D3, D4);
/*************************************************显示程序*************************************************/
String list[20];
void List(String string_) {
  memset(list, '\0', sizeof(list));
  File _list;
  _list = SPIFFS.open(string_, "r");
  String string = _list.readString();
  _list.close();
  int number = 0;
  while (string.indexOf(':') != -1) {
    list[number] = string.substring(0, string.indexOf(':'));
    string = string.substring(string.indexOf(':') + 1);
    number++;
  }
}

//以下是汉字显示程序 (文件 , x , y, 是否换行)
void display_str(String string, int display_x , int display_y , bool line_feed) {
  int x = 0;
  int y = 0;
  bool font_set = false;
  int horizontal = 0;
  int vertical = 0;
  String fs_name;
  String Chinese;
  char Char;
  File Fs_open;

  if (string.indexOf(':') != -1) {
    x = string.substring(string.indexOf(':') + 1, string.indexOf('.')).toInt();
    y = string.substring(string.indexOf('.') + 1, string.indexOf('$')).toInt();
    fs_name = "/";
    fs_name = fs_name + x + ".Ch";
    Fs_open = SPIFFS.open(fs_name, "r");
    string = Fs_open.readString();
    Fs_open.close();
    for (y ; y > 0; y--) {
      string = string.substring(string.indexOf('#') + 1);
    }
    string = string.substring(0, string.indexOf('#'));
  }

  while (string.indexOf('$') != -1) {
    x = string.substring(0, string.indexOf('^')).toInt();
    y = string.substring(string.indexOf('^') + 1, string.indexOf('$')).toInt();
    string = string.substring(string.indexOf('$') + 1);
    if (font_set) {
      display_x -= 4 ;
      font_set = false;
    }
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    switch (x) {
      case 1:
        display.setFont(Chinese_1);
        break;
      case 2:
        display.setFont(Chinese_2);
        break;
      case 3:
        display.setFont(Chinese_3);
        break;
      case 4:
        display.setFont(Chinese_4);
        break;
      case 5:
        display.setFont(Chinese_5);
        break;
      case 6:
        display.setFont(Chinese_6);
        break;
      case 7:
        display.setFont(Chinese_7);
        break;
      case 8:
        display.setFont(Chinese_8);
        break;
      case 9:
        display.setFont(Chinese_9);
        break;
      case 10:
        display.setFont(Chinese_10);
        break;
      case 11:
        display.setFont(Chinese_11);
        break;
      case 12:
        display.setFont(Chinese_12);
        break;
      case 13:
        display.setFont(Chinese_13);
        break;
      case 14:
        display.setFont(Chinese_14);
        break;
      case 15:
        display.setFont(Chinese_15);
        break;
      case 16:
        display.setFont(Chinese_16);
        break;
      case 17:
        display.setFont(Chinese_17);
        break;
      case 18:
        display.setFont(Chinese_18);
        break;
      case 19:
        display.setFont(Chinese_19);
        break;
      case 20:
        display.setFont(Chinese_20);
        break;
      case 21:
        display.setFont(Chinese_21);
        break;
      case 22:
        display.setFont(Chinese_22);
        break;
      case 23:
        display.setFont(Chinese_23);
        break;
      case 24:
        display.setFont(Chinese_24);
        break;
      case 25:
        display.setFont(Chinese_25);
        break;
      case 26:
        display.setFont(Chinese_26);
        break;
      case 27:
        display.setFont(Chinese_27);
        break;
      case 28:
        display.setFont(Chinese_28);
        break;
      case 29:
        display.setFont(Chinese_29);
        break;
      case 30:
        display.setFont(Chinese_30);
        break;
      case 31:
        display.setFont(Chinese_31);
        break;
      case 32:
        display.setFont(Chinese_32);
        break;
      case 33:
        display.setFont(Chinese_33);
        break;
      case 34:
        display.setFont(Chinese_34);
        break;
      case 35:
        display.setFont(Chinese_35);
        break;
      case 36:
        display.setFont(Chinese_36);
        break;
      case 37:
        display.setFont(Chinese_37);
        break;
      case 38:
        display.setFont(Chinese_38);
        break;
      default :
        display.setFont(Dialog_plain_12);
        font_set = true;
        break;
    }
    Char = y;
    Chinese += Char;
    if (line_feed) {
      display.drawString(display_x + horizontal * 12 , display_y + vertical * 15 , Chinese);
    }
    else {
      display.drawString(display_x + horizontal * 12 , display_y , Chinese);
    }
    horizontal++;
    if (display_x + (horizontal + 1) * 12 > 128) {
      horizontal = 0;
      vertical++;
    }
    Chinese = "";
  }
}

//初始化主程序
void setup() {
  SPIFFS.begin();
  display.init();
  display.flipScreenVertically();
  Serial.begin(9600);
  List("/menu.list");
}

void loop() {
  Serial.print("开始时间 :");
  Serial.print(millis());
  display.clear();
  display_str(list[0], 40, 0, true);
  display_str(list[1], 40, 13, true);
  display_str(list[2], 40, 26, true);
  display_str(list[3], 40, 39, true);
  display.display();
  Serial.print("  结束时间 :");
  Serial.println(millis());
}
