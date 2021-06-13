#include <FS.h>
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D4, /* data=*/ D3, /* reset=*/ U8X8_PIN_NONE);


String list[200];
String book;

bool A_ = true;
bool B_ = true;
bool C_ = true;

int Interface;
int Serial_number;

//以下为界面模式
bool menu_open = false;//菜单
bool main_Interface_open = true;//主界面
bool Recite_Word_open = false;//记单词

bool Recite_Word_English_open = false;
bool Recite_Word_Chinese_open = false;
bool Remember ;

//乐鑫的logo
unsigned char col[] = {  0xBA, 0x3B, 0x83, 0xBB, 0xB3, 0xBB, 0xEE,
                         0xFF, 0x3F, 0xC3, 0xFF, 0xF1, 0xFF, 0x7F,
                         0x83, 0x38, 0xCF, 0xEE, 0xB0, 0x80, 0xE0,
                         0x03, 0x30, 0x4F, 0x44, 0x10, 0x00, 0x40,
                         0xA3, 0x33, 0x33, 0x00, 0x33, 0x3B, 0xCE,
                         0xF7, 0x33, 0x33, 0x00, 0x13, 0x1F, 0x47,
                         0xF3, 0x33, 0x3C, 0xCF, 0x30, 0x3F, 0xCF,
                         0xF3, 0x33, 0x3C, 0x47, 0x10, 0x1F, 0x47,
                         0xF3, 0x33, 0xF3, 0xF3, 0x30, 0x3F, 0xCF,
                         0xF7, 0x33, 0xF3, 0x71, 0x10, 0x1F, 0x47,
                         0x03, 0x30, 0x00, 0xFC, 0x30, 0x80, 0xE0,
                         0x03, 0x30, 0x00, 0x7C, 0x10, 0x00, 0x40,
                         0xFF, 0x3F, 0x33, 0x33, 0xF3, 0xFF, 0xFF,
                         0xFF, 0x3F, 0x33, 0x13, 0xF1, 0xFF, 0x7F,
                         0x00, 0x00, 0xC0, 0xC0, 0x03, 0x00, 0x00,
                         0x00, 0x00, 0x40, 0xC0, 0x03, 0x00, 0x00,
                         0x83, 0xFF, 0x0F, 0xC0, 0x0F, 0xF3, 0xFC,
                         0xC7, 0xFF, 0x0F, 0xC0, 0x07, 0xF1, 0x7C,
                         0x30, 0x03, 0xC0, 0xFC, 0xC0, 0x30, 0x0C,
                         0x30, 0x03, 0x40, 0x7C, 0x40, 0x10, 0x04,
                         0x0F, 0xFF, 0x30, 0x0C, 0xF3, 0x7F, 0x1F,
                         0x0F, 0xFF, 0x30, 0x0C, 0xF1, 0x7F, 0x1F,
                         0x3F, 0x03, 0x33, 0x0F, 0xF3, 0xEE, 0xFB,
                         0x3F, 0x03, 0x33, 0x07, 0x73, 0xC4, 0x71,
                         0xCC, 0xF0, 0xCF, 0xFC, 0xEC, 0x0C, 0x03,
                         0xCC, 0xF0, 0xCF, 0x7C, 0x44, 0x04, 0x01,
                         0xCF, 0xC0, 0x30, 0x3F, 0xF0, 0x30, 0x0C,
                         0xDF, 0x41, 0x31, 0x1F, 0x70, 0x10, 0x04,
                         0xCF, 0x3C, 0x03, 0x0C, 0x03, 0x3C, 0x0F,
                         0xCF, 0x3C, 0x03, 0x0C, 0x01, 0x1C, 0x07,
                         0x33, 0x0C, 0x30, 0x3F, 0xF3, 0x00, 0x00,
                         0x33, 0x14, 0x30, 0x3F, 0x73, 0x00, 0x00,
                         0x33, 0x30, 0xFC, 0x0F, 0xFF, 0x33, 0xCC,
                         0x77, 0x30, 0xFC, 0x07, 0xFF, 0x11, 0x44,
                         0x00, 0x00, 0xCF, 0x00, 0x03, 0xCF, 0x33,
                         0x00, 0x01, 0x57, 0x01, 0x03, 0x47, 0x11,
                         0xFF, 0x3F, 0x3F, 0xCF, 0x33, 0x03, 0x00,
                         0xFF, 0x3F, 0x1F, 0xC7, 0x11, 0x01, 0x00,
                         0x03, 0x30, 0xCF, 0x30, 0x03, 0x33, 0xCC,
                         0x13, 0x31, 0x57, 0x11, 0x13, 0x11, 0x44,
                         0xF3, 0x33, 0xF3, 0x03, 0xFF, 0x3F, 0x0F,
                         0xF7, 0x33, 0xF7, 0x05, 0xFD, 0x1F, 0x07,
                         0xF3, 0x33, 0x3F, 0x0F, 0xC0, 0xCF, 0xF3,
                         0xF3, 0x33, 0x1F, 0x17, 0x40, 0xD7, 0x75,
                         0xF3, 0x33, 0xCC, 0x33, 0x00, 0xFE, 0xFF,
                         0x57, 0x31, 0xC4, 0x15, 0x04, 0xDC, 0x77,
                         0x03, 0x30, 0x30, 0x3C, 0x3C, 0xCF, 0xF3,
                         0x17, 0x31, 0x31, 0x35, 0x1C, 0x47, 0x51,
                         0xFF, 0x3F, 0xFF, 0x33, 0x3C, 0x0F, 0xC3,
                         0xD5, 0x15, 0xD5, 0x11, 0x14, 0x05, 0x41,
                      };

//菜单界面下的小手指
unsigned char arrow[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFA, 0x3E, 0x86, 0x3F, 0x42, 0x03,
                         0xBA, 0x02, 0xE2, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        };

//以下是按键去抖
//A键
bool A() {
  if (digitalRead(D5) == LOW && A_ == true) {
    A_ = false;
    return true;
  }
  else if (digitalRead(D5) == HIGH && A_ == false) {
    A_ = true;
    return false;
  }
  else {
    return false;
  }
}
//B键
bool B() {
  if (digitalRead(D6) == LOW && B_ == true) {
    B_ = false;
    return true;
  }
  else if (digitalRead(D6) == HIGH && B_ == false) {
    B_ = true;
    return false;
  }
  else {
    return false;
  }
}
//C键
bool C() {
  if (digitalRead(D7) == LOW && C_ == true) {
    C_ = false;
    return true;
  }
  else if (digitalRead(D7) == HIGH && C_ == false) {
    C_ = true;
    return false;
  }
  else {
    return false;
  }
}

//居中显示
void Center_print(String string, int y) {
  u8g2.setFont(u8g2_font_ncenB12_tr); //设置字体
  u8g2.setCursor((128 - string.length() * 12) / 2 , y); //设置光标处
  u8g2.print(string);
}

//以下是汉字显示程序 (文件 , x , y,文字大小, 是否换行)  最多换一行const int quantity
void display_str(String string_, int x, int y, const int quantity, bool line_feed) {
  String string;
  String Chinese;
  int number;
  int dif;
  unsigned char display_[quantity * 2];

  File myFile ;
  myFile = SPIFFS.open(string_ , "r");
  string = myFile.readString();
  myFile.close();
  while (string.indexOf('(') != -1) {
    string = string.substring(string.indexOf(')'));
    Chinese = string.substring(string.indexOf(')') + 1, string.indexOf('/'));
    string = string.substring(string.indexOf('/'));
    for (int i = 0; i < quantity * 2; i++) {
      display_[i] = Chinese.substring(0, Chinese.indexOf(',')).toInt();
      Chinese = Chinese.substring(Chinese.indexOf(',') + 1);
    }
    if (line_feed && x + number * quantity < 120 || !line_feed) {
      u8g2.drawXBMP(x + number * quantity , y , quantity, quantity , display_);
      dif = (number + 1) * quantity;
    }
    else {
      u8g2.drawXBMP(x + number * quantity - dif, y + 13 , quantity, quantity , display_);
    }
    number++;
  }
}

//以下读取列表
void List(String string_ , String _string) {
  memset(list, '\0', sizeof(list));
  File _list;
  _list = SPIFFS.open(string_, "r");
  String string = _list.readString();
  _list.close();
  int i = 0;
  while (string.indexOf('/') != -1) {
    list[i] = string.substring(string.indexOf('/'), string.indexOf('.') + _string.length());
    string = string.substring(string.indexOf('.') + _string.length() + 1);
    i++;
  }
}


//菜单界面
void menu() {
  if (menu_open) {
    u8g2.clearBuffer();
    u8g2.drawXBMP( 20 , -1 , 16 , 16 , arrow );
    display_str(list[Interface], 40, 0, 12, false);
    display_str(list[Interface + 1], 40, 13, 12, false);
    display_str(list[Interface + 2], 40, 26, 12, false);
    display_str(list[Interface + 3], 40, 39, 12, false);
    u8g2.sendBuffer();
    if (A()) {
      Interface--;
    }
    if (C()) {
      Interface++;
    }
    if (B()) {
      switch (Interface) {
        case 0:
          menu_open = false;
          main_Interface_open = true;
          Interface = 0;
          break;
        case 1:
          menu_open = false;
          Recite_Word_open = true;
          List("/book.list", ".bak");
          Interface = 0;
          break;
      }
    }
  }
}


//主界面
void main_Interface() {
  if (main_Interface_open) {
    u8g2.clearBuffer();
    u8g2.drawXBMP( 0 , 0 , 50 , 50 , col );
    u8g2.sendBuffer();
    if (B()) {
      menu_open = true;
      main_Interface_open = false;
      List("/menu.list", ".set");
    }
  }
}


//记单词的界面
void Recite_Word() {
  if (Recite_Word_open) {
    u8g2.clearBuffer();
    u8g2.drawXBMP( 10 , -1 , 16 , 16 , arrow );
    display_str(list[Interface], 30, 0, 12, false);
    display_str(list[Interface + 1], 30, 13, 12, false);
    display_str(list[Interface + 2], 30, 26, 12, false);
    display_str(list[Interface + 3], 30, 39, 12, false);
    u8g2.sendBuffer();
    if (A()) {
      Interface--;
    }
    if (C()) {
      Interface++;
    }
    if (B()) {
      switch (Interface) {
        case 0:
          menu_open = true;
          Recite_Word_open = false;
          List("/menu.list", ".set");
          Interface = 0;
          break;
        case 13:
          menu_open = true;
          Recite_Word_open = false;
          List("/menu.list", ".set");
          Interface = 0;
          break;
        default :
          Recite_Word_open = false;
          Recite_Word_English_open = true;
          book = list[Interface].substring(list[Interface].indexOf('/'), list[Interface].indexOf('.')) + ".list";
          List(book, ".c");
          Serial_number = 0;
          Interface = 0;
          break;
      }
    }
  }
}


//记单词主函数
void Recite_Word_Display_English() {
  if (Recite_Word_English_open) {
    u8g2.clearBuffer();
    Center_print(list[Serial_number].substring(list[Serial_number].indexOf('/') + 1, list[Serial_number].indexOf('.')), 12);
    display_str("/jizhuli.set", 28, 27, 12, false);
    display_str("/ci.set", 88, 27, 12, false);
    display_str("/weijizhu.set", 28, 40, 12, false);
    display_str("/ci.set", 88, 40, 12, false);
    u8g2.sendBuffer();
    if (A() || C()) {
      Recite_Word_English_open = false;
      Recite_Word_Chinese_open = true;
    }
    if (B()) {
      Recite_Word_open = true;
      Recite_Word_English_open = false;
      List("/book.list", ".bak");
    }
  }
}

void Recite_Word_Display_Chinese() {
  if (Recite_Word_Chinese_open) {
    u8g2.clearBuffer();
    display_str("/jizhuli.set", 0, 51, 12, false);
    display_str("/weijizhu.set", 91, 51, 12, false);
    display_str(list[Serial_number], 0, 13, 12, true);
    u8g2.sendBuffer();
    if (A() || C()) {
      Recite_Word_English_open = true;
      Recite_Word_Chinese_open = false;
      Serial_number++;
    }
    if (B()) {
      Recite_Word_open = true;
      Recite_Word_Chinese_open = false;
      List("/book.list", ".bak");
    }
  }
}


//初始化程序
void setup() {
  u8g2.begin();
  SPIFFS.begin();
  pinMode(D8, OUTPUT);
  digitalWrite(D8, LOW);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
}


void loop() {
  menu();
  main_Interface();
  Recite_Word();
  Recite_Word_Display_English();
  Recite_Word_Display_Chinese();
}
